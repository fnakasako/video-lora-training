cmake_minimum_required(VERSION 3.15)
project(VideoPipeline LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# On macOS with Homebrew, we need to tell CMake where to find packages.
if(APPLE)
  list(APPEND CMAKE_PREFIX_PATH /opt/homebrew)
endif()

# ===================================================================
# Dependencies
# ===================================================================
find_package(PkgConfig REQUIRED)
pkg_check_modules(AVCODEC REQUIRED IMPORTED_TARGET libavcodec)
pkg_check_modules(AVFORMAT REQUIRED IMPORTED_TARGET libavformat)
pkg_check_modules(SWSCALE REQUIRED IMPORTED_TARGET libswscale)


# ===================================================================
# Library Target (The Core Engine)
# ===================================================================
add_library(video_pipeline_lib
    src/video_pipeline.cpp
)

# The library's public headers are in the 'include' directory.
target_include_directories(video_pipeline_lib PUBLIC include)

# Add the 'external' directory for stb_image_write.h
# Using SYSTEM suppresses warnings from third-party headers.
target_include_directories(video_pipeline_lib SYSTEM PRIVATE external)

# Apply strict warning flags to our own code.
target_compile_options(video_pipeline_lib PRIVATE -Wall -Wextra)

# Link the library against the FFmpeg components it needs.
target_link_libraries(video_pipeline_lib PRIVATE
    PkgConfig::AVCODEC
    PkgConfig::AVFORMAT
    PkgConfig::SWSCALE
)


# ===================================================================
# Executable Target (The Command-Line Tool)
# ===================================================================
add_executable(video_pipeline
    src/main.cpp
)

# Tell the executable where to find CLI11.hpp
target_include_directories(video_pipeline PRIVATE external)

# Link the executable against our library.
target_link_libraries(video_pipeline PRIVATE video_pipeline_lib)


# ===================================================================
# Python Bindings Setup
# ===================================================================
find_package(Python REQUIRED COMPONENTS Interpreter Development)
include(FetchContent)
FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11.git
  GIT_TAG        v2.12.0
)
FetchContent_MakeAvailable(pybind11)

pybind11_add_module(video_pipeline_py
    src/bindings.cpp
)

target_link_libraries(video_pipeline_py PRIVATE video_pipeline_lib)


# ===================================================================
# Testing Setup
# ===================================================================
enable_testing()

add_library(doctest INTERFACE)
target_include_directories(doctest INTERFACE external)

add_executable(run_tests
    tests/video_pipeline.test.cpp
)

target_link_libraries(run_tests PRIVATE video_pipeline_lib doctest)

add_test(NAME VideoPipelineTests COMMAND run_tests)
