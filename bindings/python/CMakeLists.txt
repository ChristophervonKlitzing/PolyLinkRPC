# =============== Install pybind11 ===============
include(FetchContent)
FetchContent_Declare(
    pybind11
    GIT_REPOSITORY https://github.com/pybind/pybind11
    GIT_TAG        v2.2.3
)

FetchContent_GetProperties(pybind11)
if(NOT pybind11_POPULATED)
    FetchContent_Populate(pybind11)
    add_subdirectory(${pybind11_SOURCE_DIR} ${pybind11_BINARY_DIR})
endif()

# ================================================

# Create python module:
file(GLOB PYBIND_SOURCES "./src/*.cpp")
pybind11_add_module(modulePolyLinkRPC module.cpp ${PYBIND_SOURCES})
target_link_libraries(modulePolyLinkRPC PRIVATE libPolyLinkRPC)
target_compile_options(modulePolyLinkRPC PRIVATE "-Wno-deprecated")