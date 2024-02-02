#!/bin/bash

# Find all C++ files in the project, excluding the lib directory
cpp_files=$(find .. -path "../lib" -prune -o -path "../build" -prune -o \( -name "*.cpp" -o -name "*.hpp" \) -print)

# Format each C++ file using clang-format
for file in $cpp_files; do
    echo "Formatting $file"
    clang-format -i $file
done
