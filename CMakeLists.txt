# Seam Language Compiler

cmake_minimum_required(VERSION 3.5)

project(seam VERSION 0.0.1 LANGUAGES CXX)

# Set C++ Standard (this is supposedly outdated?)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/bin)

# Add other projects
add_subdirectory(seam)
add_subdirectory(example)
add_subdirectory(compiler)
add_subdirectory(test)
