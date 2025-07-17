#!/bin/bash
set -e

echo "--- Setting up Video-LoRA-Pipeline Environment using uv ---"

# --- 1. Check for System Dependencies ---
echo "[1/4] Checking for system dependencies..."
command -v cmake >/dev/null 2>&1 || { echo >&2 "Error: cmake is not installed. Please install it."; exit 1; }
command -v brew >/dev/null 2>&1 || { echo >&2 "Warning: Homebrew not found. Please ensure ffmpeg is installed manually."; }
if command -v brew >/dev/null 2>&1; then
    if ! brew ls --versions ffmpeg > /dev/null; then
        echo "ffmpeg not found, attempting to install with Homebrew..."
        brew install ffmpeg
    fi
fi
command -v uv >/dev/null 2>&1 || { echo >&2 "Error: uv is not installed. Please install it via 'pip install uv' or see https://github.com/astral-sh/uv"; exit 1; }
echo "System dependencies look good."

# --- 2. Create Python Virtual Environment with uv ---
echo "[2/4] Creating Python virtual environment in '.venv' using uv..."
uv venv
source .venv/bin/activate
echo "Virtual environment created and activated."

# --- 3. Install Python Dependencies with uv ---
echo "[3/4] Installing Python packages from requirements.txt using uv..."
uv pip install -r requirements.txt
echo "Python packages installed."

# --- 4. Set up Environment File for API Key ---
echo "[4/4] Setting up environment file for API key..."
if [ ! -f ".env" ]; then
    echo "GOOGLE_API_KEY=\"YOUR_API_KEY_HERE\"" > .env
    echo "Created .env file. Please edit it and add your Google AI Studio API key."
else
    echo ".env file already exists. Please ensure your GOOGLE_API_KEY is set."
fi

echo -e "\n--- Setup Complete! ---"
echo "To activate the environment in the future, run: source .venv/bin/activate"
echo "Next steps:"
echo "1. Edit the .env file to add your API key."
echo "2. Create the build directory: cmake -B build"
echo "3. Compile the project: cmake --build build"
