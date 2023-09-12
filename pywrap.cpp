#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "src/mylibnn.h"

namespace py = pybind11;
constexpr auto byref = py::return_value_policy::reference_internal;

PYBIND11_MODULE(MyLibNN, m) {
    m.doc() = "optional module docstring";

    py::class_<Tree>(m, "Tree")
    .def(py::init<std::vector<std::vector<int>>, std::vector<int>, int, int>())
    .def("print_num_points", &Tree::print_num_points, py::call_guard<py::gil_scoped_release>())
    .def("getNeighbors", &Tree::getNeighbors, py::arg("radius"), py::arg("start"), py::arg("end"), py::call_guard<py::gil_scoped_release>())
    .def_readonly("return_matrix", &Tree::return_matrix, byref)
    ;
}
