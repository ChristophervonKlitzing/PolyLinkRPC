# Bash script for computing the test-coverage and visualizing it using html

cmake -DCOMPUTE_CODE_COVERAGE=ON -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --target unit_tests
ctest --test-dir build # creates coverage data

# Generate coverage output
gcovr --root . --html-details build/coverage_html/ --html-details-syntax-highlighting \
  --filter 'src/.*\.cpp$' \
  --filter 'include/.*/.*\.hpp$' \
  --exclude 'src/main.cpp'