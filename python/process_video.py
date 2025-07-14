import argparse
import os
import sys

# How to run this script:
# 1. Build the C++ project to create the C++ module.
# 2. Navigate to your 'build' directory in the terminal:
#    cd /path/to/your/project/build
# 3. Run this script from the 'build' directory:
#    python ../python/process_video.py /path/to/video.mp4 /path/to/output_frames --total-frames 150

# Add the build directory to the Python path so that it can find our C++ module. 
# This makes the script runnable from anywhere, but running from 'build' is simplest.
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'build')))

# Import the C++ module.
try:
    import video_pipeline_py
except:
    ImportError:
    print("Error: Could not import the C++ Module 'video_pipeline_pu'")
    print("Please make sure you have built the C++ project and are running this script correctly.")
    sys.exit(1)

def main():
    """
    Main function to parse arguments and run the video processor.
    """
    parser = argparse(ArgumentParser(
        description="A high-level Python script to process videos into frames using C++ core."
    ))

    # Define CLI Arguments
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
        action="store_true", # This makes it a simple boolean flag
        help="If specified, crop the center square of the frame before resizing."
    )

    args = parser.parse)args()

    print("--- Starting Video Processing ---")
    print(f"Input Video: {args.input_path}")
    print(f"Output Directory: {args.output_dir}")
    print(f"Frames to Extract: {args.total_frames}")
    print(f"Output Size: {args.size}x{args.size}")
    print(f"Center Crop: {'Yes' if args.center_crop else 'No'}")
    print("---------------------------------")

    try:
        # Call the C++ Core.
        print("\nCalling C++ core...")

        # Create an instance of the C++ VideoProcessor class.
        processor = video_pipeline_py(
            args.input_path,
            args.output_dir,
            args.center_crop,
            args.total_drames,
            args.size
        )

        # Call the C++ process() method.

        processor.process()

        print("\nC++ core finished processing successfully.")

    except Exception as e:
        # Catch any exceptions that might bubble up from the C++ code.
        print(f"\nAn error occurred during C++ processing: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()