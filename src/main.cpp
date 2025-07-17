#include <iostream>
#include <string>
#include <chrono>
#include <filesystem>
#include "cli/CLI11.hpp"
#include "video_pipeline.h"

int main(int argc, char** argv) {
    CLI::App app("A command-line tool to turn videos into frames for model training.");
    app.require_subcommand(1);

    CLI::App* process_subcommand = app.add_subcommand("process", "Process a video.");

    // --- Variables to hold the command-line arguments ---
    std::string input_path;
    std::string output_dir;
    bool center_crop = false;
    int total_frames = 0;    // Default to processing all frames
    int frame_width = -1;    // Default to original video width
    int frame_height = -1;   // Default to original video height

    // --- Define the command-line arguments and options ---
    
    // Required positional arguments
    process_subcommand->add_option("input_path", input_path, "Path to the input video file.")->required();
    process_subcommand->add_option("output_dir", output_dir, "Directory to save the output frames.")->required();

    // Optional flags with values
    process_subcommand->add_option("--total-frames", total_frames, "Total frames to extract. 0 means all frames.");
    process_subcommand->add_option("--width", frame_width, "Output width. Defaults to original video width.");
    process_subcommand->add_option("--height", frame_height, "Output height. Defaults to original video height.");
    
    // Optional boolean flag
    process_subcommand->add_flag("--center-crop", center_crop, "Crop the center square before resizing.");

    CLI11_PARSE(app, argc, argv);

    // --- Run the main logic ---
    if (app.got_subcommand("process")) {
        try {
            std::filesystem::create_directories(output_dir);

            auto start_time = std::chrono::high_resolution_clock::now();

            // Create an instance of our processor with the new constructor signature.
            VideoProcessor processor(
                input_path,
                output_dir,
                center_crop,
                total_frames,
                frame_width,
                frame_height
            );
            
            processor.process();

            auto end_time = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);
            std::cout << "\nTotal processing time: " << duration.count() << " seconds." << std::endl;

        } catch (const std::exception& e) {
            std::cerr << "An error occurred: " << e.what() << std::endl;
            return 1; // Indicate failure
        }
    }

    return 0; // Success
}