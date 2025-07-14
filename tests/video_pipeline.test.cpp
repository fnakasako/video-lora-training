#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include "video_pipeline.h"
#include <filesystem>
#include <vector> 

// Your test cases will go here later

int count_files_in_directory(const std::filesystem::path& directory) {
    int count = 0;
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            count++;
        }
    }
    return count;
}

TEST_CASE("VideoProcessor correctly extracts a specific number of frames") {
    // --- 1. Setup ---
    const std::filesystem::path test_video_path = "test_data/test_clip.mp4";
    const std::filesystem::path temp_output_dir = "temp_test_output";
    const int frames_to_extract = 10;

    // Ensure the test video exists before running the test
    REQUIRE(std::filesystem::exists(test_video_path));

    // Create a clean temporary directory for the output frames
    if (std::filesystem::exists(temp_output_dir)) {
        std::filesystem::remove_all(temp_output_dir);
    }
    std::filesystem::create_directory(temp_output_dir);

    // --- 2. Execution ---
    try {
        // Create and run the processor with our test settings
        VideoProcessor processor(
            test_video_path,
            temp_output_dir,
            false, // center_crop
            frames_to_extract,
            128    // frame_size (small for a fast test)
        );
        processor.process();

    } catch (const std::exception& e) {
        // If the process throws an error, fail the test immediately
        FAIL("VideoProcessor::process() threw an unexpected exception: ", e.what());
    }

    // --- 3. Assertion ---
    // Check if the correct number of PNG files were created
    int output_file_count = count_files_in_directory(temp_output_dir);
    CHECK(output_file_count == frames_to_extract);

    // --- 4. Teardown ---
    // Clean up the temporary directory and its contents
    std::filesystem::remove_all(temp_output_dir);
}