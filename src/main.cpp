#include <iostream>
#include <string>
#include <chrono>
#include "cli/CLI11.hpp"
#include "video_pipeline.h"

int main(int argc, char** argv) {
    CLI::App app("A command-line tool to turn videos into frames for model training.");
    app.require_subcommand(1);

    CLI11::App* process_subcommand = app.add_subcommand("process", "Process a video.");

    std::string input_path;
    std::string output_path;
    bool center_crop = true;
    int total_frames = 100;
    int frame_size = 512;

    # Required arguments.
    process_subcommand->add_option("input_path", input_path, "Path to input video file.")->required();
    process_subcommand->add_option("output_path", output_path, "Path for video file output.")->required();

    # Optional flags with values.
    process_subcommand -> add_option("frames", total_frames, "Set the number of frames produced.", true );
    process_subcommand -> add_option("frame_size", frame_size, "Set the size of the frames produced. General values are 512x512 or 768x768.", true);

    # Optional boolean flag.
    process_subcommand -> add_option("center_crop", center_crop, "Decide whether or not to center crop rather than fitting image to dimensions.");

    CLI11_PARSE(app, argc, argv);

    // Run the main logic. 
    // Only runs if the "process" subcommand was used.
    if (app.got_subcommand("process")) {
        try {
            // Create the output directory if it doesn't exist
            std::filesystem::create_directories(output_dir);

            // Start the timer
            auto start_time = std::chrono::high_resolution_clock::now();

            // Create an instance of the processor with user's settings.
            VideoProcessor processor(input_path, output_path, center_crop, total_frames, frame_size);

            process.process();

            // Stop the timer and print the duration.
            auto end_time = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);
            std::cout << "\nTotal processing time:" << duration.count() << "seconds."
        } catch (const std::exception& e) {
            // Catch any errors thrown by the library and print them gracefully.
            std::cerr <<"An error occurred:" << e.what() << std::endl;
            return 1;
        }
    }

    return 0;
}