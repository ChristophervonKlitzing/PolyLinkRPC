option(PYTHON_TARGETS "Add python targets" OFF)
if (PYTHON_TARGETS)
  message("Include python subdirectory")
  add_subdirectory(python)
endif(PYTHON_TARGETS)
