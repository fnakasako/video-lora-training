Video to LoRA Training Pipeline
This project provides a high-performance pipeline to convert video files into a captioned image dataset suitable for LoRA training. It uses a C++ core for fast video decoding and frame extraction, orchestrated by a user-friendly Python script.
Features

High-Performance C++ Core: Uses FFmpeg to rapidly decode video and extract frames.
Python Orchestration: A simple Python script manages the end-to-end workflow.
Automatic Captioning: Optionally uses the Google Gemini API to generate descriptive captions for each frame.
Flexible Frame Extraction: Allows users to specify the total number of frames, output size, and cropping method.
User-Friendly Interface: Supports both command-line arguments and a GUI file selector for ease of use.

Prerequisites

CMake: For building the C++ project.
FFmpeg: The core video processing library. On macOS, this can be installed via Homebrew (brew install ffmpeg).
uv: For fast Python environment and package management. Install with pip install uv.
A C++ Compiler: (e.g., Clang on macOS, GCC on Linux, MSVC on Windows).

1. Setup
First, run the setup script to prepare the environment. This will check for dependencies, create a Python virtual environment, and install the necessary packages.
bash# Make the script executable (only need to do this once)
chmod +x setup.sh

# Run the setup process
./setup.sh
After the script completes, it will create a .env file in the project root.
Important: You must edit the .env file and add your Google AI Studio API key for the captioning feature to work.
bash# .env
GOOGLE_API_KEY="YOUR_API_KEY_HERE"
2. Build the C++ Module
Next, compile the C++ library and the Python bindings using CMake.
bash# Configure the build
cmake -B build

# Compile the project
cmake --build build
This will create a video_pipeline_py.so (or similar) file inside the build directory, which is the C++ module that Python will import.
3. Usage
To run the pipeline, you must execute the Python script from within the build directory.
bash# Navigate into the build directory
cd build
Command-Line Mode
Provide the input and output paths directly, along with any desired options.
python ../python/process_video.py "/path/to/my/video.mp4" "/path/to/output_folder" --total-frames 150 --size 768 --caption --center-crop
GUI Mode (for File Selection)
To use the native file selector to choose your video and output directory, simply run the script without the path arguments.
python ../python/process_video.py --total-frames 150 --size 768 --caption
The script will then open a file dialog for you to select the input video, followed by a directory dialog for the output location.