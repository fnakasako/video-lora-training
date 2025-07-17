#pragma once

#include <filesystem>
#include <string>
#include <cstdint>

// Processes a video file to extract and save frames for AI dataset creation.
class VideoProcessor {
public:
    // Constructor: Initializes the processor with all necessary settings.
    // frame_width and frame_height are optional. If <= 0, the original video dimensions are used.
    VideoProcessor(
        const std::filesystem::path& input_path,
        const std::filesystem::path& output_dir,
        bool center_crop,
        int total_frames,
        int frame_width = -1, // Optional: defaults to original
        int frame_height = -1 // Optional: defaults to original
    );

    // The main public method to start the frame extraction process.
    void process();

private:
    // Private member variables to store the configuration.
    std::filesystem::path input_path_;
    std::filesystem::path output_dir_;
    bool center_crop_;
    int total_frames_;
    int frame_width_;
    int frame_height_;
};
