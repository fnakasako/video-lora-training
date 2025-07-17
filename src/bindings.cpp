#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "video_pipeline.h"

namespace py = pybind11;

PYBIND11_MODULE(video_pipeline_py, m) {
    m.doc() = "A high-performance C++ video processing library for Python.";

    py::class_<VideoProcessor>(m, "VideoProcessor")
        .def(py::init<
            const std::string&,
            const std::string&,
            bool,
            int,
            int, // frame_width
            int  // frame_height
        >(), 
        py::arg("input_path"), 
        py::arg("output_dir"), 
        py::arg("center_crop"), 
        py::arg("total_frames"),
        py::arg("frame_width") = -1, // Make optional in Python
        py::arg("frame_height") = -1 // Make optional in Python
        )
        .def("process", &VideoProcessor::process);
}
