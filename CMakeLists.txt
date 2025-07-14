cmake_minimum_required(VERSION 3.15)
project(VideoPipeline LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# On macOS with Homebrew, we need to tell CMake where to find packages.
# This adds Homebrew's default install location to CMake's search path.
if(APPLE)
  list(APPEND CMAKE_PREFIX_PATH /opt/homebrew)
endif()

# ===================================================================
# Dependencies
# ===================================================================
# Use PkgConfig to find FFmpeg components, which is more reliable.
find_package(PkgConfig REQUIRED)
pkg_check_modules(AVCODEC REQUIRED IMPORTED_TARGET libavcodec)
pkg_check_modules(AVFORMAT REQUIRED IMPORTED_TARGET libavformat)
pkg_check_modules(SWSCALE REQUIRED IMPORTED_TARGET libswscale)


# ===================================================================
# Library Target (The Core Engine)
# ===================================================================
# All core logic is compiled into this reusable library.
add_library(video_pipeline_lib
    src/video_pipeline.cpp
)

# The library's public headers are in the 'include' directory.
# PUBLIC means that any target linking to this library will automatically
# get this include path.
target_include_directories(video_pipeline_lib PUBLIC include)

# Apply strict warning flags to the library.
target_compile_options(video_pipeline_lib PRIVATE -Wall -Wextra)

# Link the library against the FFmpeg components found by PkgConfig.
# Linking to these imported targets also handles include directories automatically.
target_link_libraries(video_pipeline_lib PRIVATE
    PkgConfig::AVCODEC
    PkgConfig::AVFORMAT
    PkgConfig::SWSCALE
)


# ===================================================================
# Executable Target (The Command-Line Tool)
# ===================================================================
# A thin executable that uses the library.
add_executable(video_pipeline
    src/main.cpp
)

# Link the executable against our library to use its functionality.
target_link_libraries(video_pipeline PRIVATE video_pipeline_lib)


# ===================================================================
# Testing Setup
# ===================================================================
enable_testing()

# Define a "virtual" library for doctest.
add_library(doctest INTERFACE)
target_include_directories(doctest INTERFACE external)

# Create the test executable.
add_executable(run_tests
    tests/video_pipeline.test.cpp
)

# Link the test runner against our main library and doctest.
target_link_libraries(run_tests PRIVATE video_pipeline_lib doctest)

# Register the test with CTest.
add_test(NAME VideoPipelineTests COMMAND run_tests)