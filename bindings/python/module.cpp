#include <pybind11/pybind11.h>

#include "PolyLinkRPC/task.h"
#include "PolyLinkRPC/versions.h"
#include "bind_declarations.h"

int add(int i, int j) { return i + j; }

namespace py = pybind11;

PYBIND11_MODULE(modulePolyLinkRPC, m) {
  m.doc() = R"pbdoc(
        Pybind11 example plugin
        -----------------------

        .. currentmodule:: cmake_example

        .. autosummary::
           :toctree: _generate

           add
           subtract
    )pbdoc";

  m.def("add", &add, R"pbdoc(
        Add two numbers

        Some other explanation about the add function.
    )pbdoc");

  m.def(
      "subtract", [](int i, int j) { return i - j; }, R"pbdoc(
        Subtract two numbers

        Some other explanation about the subtract function.
    )pbdoc");

  bind_bytes_buffer(m);

  m.attr("__version__") = VERSION_STRING;
}