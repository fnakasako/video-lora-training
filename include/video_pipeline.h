#pragma once

#include <filesystem>
#include <string>
#include <cstdint>

// Manages the discovery and retrieval of AI model files from the filesystem.
class VideoProcessor {
public:
    // Constructor: Initializes the processor with all necessary settings.
    VideoProcessor(
        const std::filesystem::path& input_path,
        const std::filesystem::path& output_dir,
        bool center_crop,
        int total_frames,
        int frame_size
    );

    // Main public method to start the frame extraction process.
    void process();

private:
    // Private member variables to store the configuration.
    std::filesystem::path input_path_;
    std::filesystem::path output_dir_;
    bool center_crop_;
    int total_frames_;
    int frame_size_;
}; // A semicolon is required at the end of a class definition.
