#pragma once

#include <string>
#include <filesystem>
#include <cstdint>

struct VideoProcessor {
    std::filesystem::path input_path;
    std::filesystem::path output_path;
    int total_frames;
};

class VideoProcessor {
public:
    // Constructor for intializing processor.
    VideoProcessor{
        const std::filesystem::path& input_path,
        const std::filesystem::path& output_path,
        bool center_crop,
        int total_frames,
        int frame_size
    };

    // Main public method to start frame extraction.
    void process();

private:
    const std::filesystem::path input_path;
    const std::filesystem::path output_path;
    bool center_crop,
    int total_frames;
    int frame_size;

}