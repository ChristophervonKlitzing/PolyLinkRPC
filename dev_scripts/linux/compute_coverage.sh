source $(dirname "$0")/build_debug.sh

# create coverage data
ctest --test-dir build  

# Generate coverage output
gcovr --root . --html-details build/coverage_html/ --exclude-unreachable-branches --exclude-throw-branches --html-details-syntax-highlighting --branches --txt \
  --filter 'src/.*\.cpp$' \
  --filter 'include/.*/.*\.hpp$' \
  --exclude 'src/main.cpp'