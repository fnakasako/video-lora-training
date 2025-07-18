# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 4.0

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/homebrew/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/frank/Desktop/company/video-lora-training/video-lora-training

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/frank/Desktop/company/video-lora-training/video-lora-training/build

# Include any dependencies generated for this target.
include CMakeFiles/run_tests.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/run_tests.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/run_tests.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/run_tests.dir/flags.make

CMakeFiles/run_tests.dir/codegen:
.PHONY : CMakeFiles/run_tests.dir/codegen

CMakeFiles/run_tests.dir/tests/video_pipeline.test.cpp.o: CMakeFiles/run_tests.dir/flags.make
CMakeFiles/run_tests.dir/tests/video_pipeline.test.cpp.o: /Users/frank/Desktop/company/video-lora-training/video-lora-training/tests/video_pipeline.test.cpp
CMakeFiles/run_tests.dir/tests/video_pipeline.test.cpp.o: CMakeFiles/run_tests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/frank/Desktop/company/video-lora-training/video-lora-training/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/run_tests.dir/tests/video_pipeline.test.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/run_tests.dir/tests/video_pipeline.test.cpp.o -MF CMakeFiles/run_tests.dir/tests/video_pipeline.test.cpp.o.d -o CMakeFiles/run_tests.dir/tests/video_pipeline.test.cpp.o -c /Users/frank/Desktop/company/video-lora-training/video-lora-training/tests/video_pipeline.test.cpp

CMakeFiles/run_tests.dir/tests/video_pipeline.test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/run_tests.dir/tests/video_pipeline.test.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/frank/Desktop/company/video-lora-training/video-lora-training/tests/video_pipeline.test.cpp > CMakeFiles/run_tests.dir/tests/video_pipeline.test.cpp.i

CMakeFiles/run_tests.dir/tests/video_pipeline.test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/run_tests.dir/tests/video_pipeline.test.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/frank/Desktop/company/video-lora-training/video-lora-training/tests/video_pipeline.test.cpp -o CMakeFiles/run_tests.dir/tests/video_pipeline.test.cpp.s

# Object files for target run_tests
run_tests_OBJECTS = \
"CMakeFiles/run_tests.dir/tests/video_pipeline.test.cpp.o"

# External object files for target run_tests
run_tests_EXTERNAL_OBJECTS =

run_tests: CMakeFiles/run_tests.dir/tests/video_pipeline.test.cpp.o
run_tests: CMakeFiles/run_tests.dir/build.make
run_tests: libvideo_pipeline_lib.a
run_tests: /opt/homebrew/Cellar/ffmpeg/7.1.1_3/lib/libavcodec.dylib
run_tests: /opt/homebrew/Cellar/ffmpeg/7.1.1_3/lib/libavformat.dylib
run_tests: /opt/homebrew/Cellar/ffmpeg/7.1.1_3/lib/libswscale.dylib
run_tests: CMakeFiles/run_tests.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/frank/Desktop/company/video-lora-training/video-lora-training/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable run_tests"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/run_tests.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/run_tests.dir/build: run_tests
.PHONY : CMakeFiles/run_tests.dir/build

CMakeFiles/run_tests.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/run_tests.dir/cmake_clean.cmake
.PHONY : CMakeFiles/run_tests.dir/clean

CMakeFiles/run_tests.dir/depend:
	cd /Users/frank/Desktop/company/video-lora-training/video-lora-training/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/frank/Desktop/company/video-lora-training/video-lora-training /Users/frank/Desktop/company/video-lora-training/video-lora-training /Users/frank/Desktop/company/video-lora-training/video-lora-training/build /Users/frank/Desktop/company/video-lora-training/video-lora-training/build /Users/frank/Desktop/company/video-lora-training/video-lora-training/build/CMakeFiles/run_tests.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/run_tests.dir/depend

