#!/bin/bash

# Find all C++ files in the project, excluding the lib directory
cpp_files=$(find .. -path "../lib" -o -prune -name "*.cpp" -o -name "*.hpp" )

# Format each C++ file using clang-format
for file in $cpp_files; do
    clang-format -i $file
done
