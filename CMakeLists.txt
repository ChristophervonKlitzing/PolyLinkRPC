cmake_minimum_required(VERSION 3.10)
project(PolyLinkRPC)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)

# Find and link against the thread library
find_package(Threads REQUIRED)
set(THREADS_PREFER_PTHREAD_FLAG ON)

# Add subdirectory for source files and test files
add_subdirectory(src)
add_subdirectory(tests)



message("Sourcefiles: ${SOURCES}")
set(HEADER_FILE_DIR ${CMAKE_SOURCE_DIR}/include/)
message("Header file directory: ${HEADER_FILE_DIR}")


# Build executable
add_executable(app ${SOURCES})
target_include_directories(app PRIVATE ${HEADER_FILE_DIR})


# Build library
add_library(libPolyLinkRPC STATIC ${SOURCES})
target_include_directories(libPolyLinkRPC PUBLIC ${HEADER_FILE_DIR})
target_link_libraries(libPolyLinkRPC PRIVATE Threads::Threads)

# Build bindings which depend on the library
add_subdirectory(bindings)

# Specifies custom install directory e.g. for development
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install")

# Specify the installation destination for the library
install(TARGETS libPolyLinkRPC
        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

# Specify the installation destination for the header files
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include
        FILES_MATCHING PATTERN "*.h")


# =========== Install dependencies ===========
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


# =========== Allow Code-Coverage ===========
option(COMPUTE_CODE_COVERAGE "Compute the code-coverage" OFF)

if (COMPUTE_CODE_COVERAGE)
  message("Code coverage on")
  include(CodeCoverage.cmake)
  append_coverage_compiler_flags()
endif(COMPUTE_CODE_COVERAGE)

enable_testing()


# =============== Unit-testing ==============:

# Add your test executable
add_executable(unit_tests ${TEST_SOURCES}) # List all test files here
target_include_directories(unit_tests PRIVATE include/)

# Link Google Test and your project's library to the test executable
target_link_libraries(unit_tests gtest gtest_main libPolyLinkRPC)

# Add the tests to CTest
add_test(NAME PolyLinkRPCTests COMMAND unit_tests)


