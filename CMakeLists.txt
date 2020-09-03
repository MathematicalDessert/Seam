# Seam Language Compiler

cmake_minimum_required(VERSION 3.10)

project(seam VERSION 0.0.1 LANGUAGES CXX)

# Set C++ Standard (this is supposedly outdated?)
set(CMAKE_CXX_STANDARD 17)

# Add other projects
add_subdirectory(core)
add_subdirectory(compiler)
add_subdirectory(test)