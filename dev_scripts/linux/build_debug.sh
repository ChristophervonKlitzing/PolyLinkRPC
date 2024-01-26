# Bash script for Building the unit-test executable in debug mode
cmake -DCOMPUTE_CODE_COVERAGE=ON -B build -DCMAKE_BUILD_TYPE=Debug -D CMAKE_CXX_FLAGS="-g"
cmake --build build --target unit_tests
