source $(dirname "$0")/build_debug.sh

valgrind --leak-check=yes --error-exitcode=1 build/unit_tests 