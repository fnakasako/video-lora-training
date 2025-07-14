#include <iostream>
#include <string>
#include <chrono>
#include "cli/CLI11.hpp"
#include "video_pipeline.h"

int main(int argc, char** argv) {
    CLI::App app("A command-line tool to turn videos into frames for model training.");

    std::string input_path;
    std::string output_path;
    bool center_crop;
    int total_frames;
    int frame_size;

    CLI11::App* process_subcommand = app.add_subcommand("process", "Process a video.");

    process_subcommand -> add_option("frames", total_frames, "Set the number of frames produced." );
    process_subcommand -> add_option("frame_size", frame_size, "Set the size of the frames produced. General values are 512x512 or 768x768.")
    process_subcommand -> add_option("center_crop", center_crop, "Decide whether or not to center crop rather than fitting image to dimensions.");

    CLI11_PARSE(app, argc, argv);

    return 0;
}