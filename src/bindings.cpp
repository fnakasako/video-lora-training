#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // Needed for automatic conversion of std::vector, std::string, etc.
#include <pybind11/filesystem.h> // Needed for automatic conversion of std::filesystem::path
#include "video_pipeline.h" // Include the C++ class we want to expose

namespace py = pybind11;

// This is the macro that creates the entry point for the Python module.
// The first argument, "video_pipeline_py", must match the name we gave the module in CMakeLists.txt.
// The second argument, "m", is the variable that represents the module object.
PYBIND11_MODULE(video_pipeline_py, m) {
    // Optional: Add a docstring to describe what the module does.
    m.doc() = "A high-performance C++ video processing library for Python.";

    // This is where we define the binding for our VideoProcessor class.
    // We are telling pybind11 to create a new Python class named "VideoProcessor".
    py::class_<VideoProcessor>(m, "VideoProcessor")
        // .def(py::init<...>()); defines the constructor. We tell it the types
        // of the arguments so Python knows how to call it.
        .def(py::init<
            const std::filesystem::path&,
            const std::filesystem::path&,
            bool,
            int,
            int
        >())

        // .def(...); defines a public method. We tell it the name of the Python
        // method ("process") and which C++ member function to call (&VideoProcessor::process).
        .def("process", &VideoProcessor::process);
}