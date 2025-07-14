import argparse
import os
import sys
import glob
from pathlib import Path

# We need to add the project's root and build directory to the Python path
# to allow importing from our other modules.
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'build')))

try:
    # This is where we import our C++ module.
    import video_pipeline_py
    # This is where we import our new Python captioning module.
    from python.captioning import generate_caption
except ImportError as e:
    print(f"Error: Could not import a required module: {e}")
    print("Please make sure you have built the C++ project and are running this script correctly.")
    sys.exit(1)


def caption_frames(output_dir: str):
    """
    Finds all PNG frames in the output directory and generates a .txt caption file for each.
    """
    print("\n--- Starting Automatic Captioning ---")
    
    image_paths = glob.glob(os.path.join(output_dir, "*.png"))
    
    if not image_paths:
        print("Warning: No frames found to caption.")
        return

    # Sort the frames numerically to process them in order
    image_paths.sort(key=lambda f: int(Path(f).stem.split('_')[-1]))

    for image_path in image_paths:
        # This now calls the powerful generate_caption function from our other file.
        caption = generate_caption(image_path)
        
        caption_path = Path(image_path).with_suffix(".txt")
        
        with open(caption_path, "w") as f:
            f.write(caption)
            
    print(f"Finished generating {len(image_paths)} captions.")


def main():
    """
    Main function to parse arguments and run the video processor.
    """
    parser = argparse.ArgumentParser(
        description="A high-level Python script to process videos into frames using a C++ core."
    )

    # --- Define Command-Line Arguments ---
    parser.add_argument(
        "input_path",
        type=str,
        help="Path to the input video file."
    )
    parser.add_argument(
        "output_dir",
        type=str,
        help="Directory to save the output frames."
    )
    parser.add_argument(
        "--total-frames",
        type=int,
        default=100,
        help="Total number of frames to extract (default: 100)."
    )
    parser.add_argument(
        "--size",
        type=int,
        default=512,
        help="The output resolution for the square frames (default: 512)."
    )
    parser.add_argument(
        "--center-crop",
        action="store_true",
        help="If specified, crop the center square of the frame before resizing."
    )
    parser.add_argument(
        "--caption",
        action="store_true",
        help="If specified, automatically generate captions for the extracted frames using Gemini."
    )

    args = parser.parse_args()

    print("--- Starting Video Processing ---")
    print(f"Input Video: {args.input_path}")
    print(f"Output Directory: {args.output_dir}")
    print(f"Frames to Extract: {args.total_frames}")
    print(f"Output Size: {args.size}x{args.size}")
    print(f"Center Crop: {'Yes' if args.center_crop else 'No'}")
    print("---------------------------------")

    try:
        # --- Call the C++ Core ---
        print("\nCalling high-performance C++ core...")

        processor = video_pipeline_py.VideoProcessor(
            args.input_path,
            args.output_dir,
            args.center_crop,
            args.total_frames,
            args.size
        )

        processor.process()

        print("\nC++ core finished processing successfully.")

    except Exception as e:
        print(f"\nAn error occurred during C++ processing: {e}")
        sys.exit(1)

    # --- Optional Captioning Step ---
    if args.caption:
        caption_frames(args.output_dir)


if __name__ == "__main__":
    main()
