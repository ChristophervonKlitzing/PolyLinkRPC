#include "../bind_declarations.h"

void bind_bytes_buffer(pybind11::module &m) {
  m.attr("test") = "Hello from C++";
}