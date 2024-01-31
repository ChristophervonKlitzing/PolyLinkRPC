# Bash script for Building the python bindings in debug mode
cmake -DPYTHON_TARGETS=ON -B build -DCMAKE_BUILD_TYPE=Debug -D CMAKE_CXX_FLAGS="-g"
cmake --build build --target modulePolyLinkRPC