cmake_minimum_required(VERSION 3.10)
project(PolyLinkRPC)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)

# Add subdirectory for source files and test files
add_subdirectory(src)
add_subdirectory(tests)

message("Sourcefiles: ${SOURCES}")


# Build executable
add_executable(app ${SOURCES})
target_include_directories(app PRIVATE include/)


# Build library
add_library(libPolyLinkRPC STATIC ${SOURCES})
target_include_directories(libPolyLinkRPC PRIVATE include/)

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


# Unit-testing
find_package(GTest)

# Add your test executable
add_executable(unit_tests ${TEST_SOURCES}) # List all test files here
target_include_directories(unit_tests PRIVATE include/)

# Link Google Test and your project's library to the test executable
target_link_libraries(unit_tests gtest gtest_main libPolyLinkRPC)

# Add the tests to CTest
add_test(NAME PolyLinkRPCTests COMMAND unit_tests)