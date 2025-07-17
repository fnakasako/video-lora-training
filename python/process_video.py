import argparse
import os
import sys
import glob
from pathlib import Path
import tkinter as tk
from tkinter import filedialog

# We need to add the project's root and build directory to the Python path
# to allow importing from our other modules.
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'build')))

try:
    # This is where we import our C++ module.
    import video_pipeline_py
    # This is where we import our Python captioning module.
    from python.captioning import generate_caption
except ImportError as e:
    print(f"Error: Could not import a required module: {e}")
    print("Please make sure you have built the C++ project and are running this script correctly.")
    sys.exit(1)


def get_paths_from_gui():
    """Opens file dialogs to get the input video and output directory."""
    root = tk.Tk()
    root.withdraw()  # Hide the main tkinter window

    print("No input path provided. Opening file selector...")
    input_path = filedialog.askopenfilename(
        title="Select a Video File",
        filetypes=[("Video Files", "*.mp4 *.mov *.avi *.mkv"), ("All files", "*.*")]
    )
    if not input_path:
        print("No file selected. Exiting.")
        sys.exit(0)

    print("Opening directory selector for output...")
    output_dir = filedialog.askdirectory(
        title="Select a Folder to Save Frames"
    )
    if not output_dir:
        print("No output directory selected. Exiting.")
        sys.exit(0)

    return input_path, output_dir


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
        nargs='?',
        default=None,
        help="Path to the input video file. (Optional: opens GUI)"
    )
    parser.add_argument(
        "output_dir",
        type=str,
        nargs='?',
        default=None,
        help="Directory to save frames. (Optional: opens GUI)"
    )
    parser.add_argument(
        "--total-frames",
        type=int,
        default=0,
        help="Total frames to extract. 0 means all frames. (default: 0)"
    )
    parser.add_argument(
        "--width",
        type=int,
        default=-1,
        help="Output width. Defaults to original video width."
    )
    parser.add_argument(
        "--height",
        type=int,
        default=-1,
        help="Output height. Defaults to original video height."
    )
    parser.add_argument(
        "--center-crop",
        action="store_true",
        help="Crop the center square before resizing."
    )
    parser.add_argument(
        "--caption",
        action="store_true",
        help="Automatically generate captions for extracted frames."
    )
    
    args = parser.parse_args()
    input_path, output_dir = args.input_path, args.output_dir
    if not input_path or not output_dir:
        input_path, output_dir = get_paths_from_gui()

    print("--- Starting Video Processing ---")
    print(f"Input Video: {input_path}")
    print(f"Output Directory: {output_dir}")
    print(f"Frames to Extract: {'All' if args.total_frames <= 0 else args.total_frames}")
    print(f"Output Width: {'Original' if args.width <= 0 else args.width}")
    print(f"Output Height: {'Original' if args.height <= 0 else args.height}")
    print(f"Center Crop: {'Yes' if args.center_crop else 'No'}")
    print("---------------------------------")

    try:
        print("\nCalling high-performance C++ core...")
        # Call the C++ constructor with the correct 6 arguments
        processor = video_pipeline_py.VideoProcessor(
            input_path,
            output_dir,
            args.center_crop,
            args.total_frames,
            args.width,
            args.height
        )
        processor.process()
        print("\nC++ core finished processing successfully.")
    except Exception as e:
        print(f"\nAn error occurred during C++ processing: {e}")
        sys.exit(1)

    if args.caption:
        caption_frames(output_dir)

if __name__ == "__main__":
    main()
