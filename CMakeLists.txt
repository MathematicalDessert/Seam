# CMakeList.txt : CMake project for Seam, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("Seam")

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(core)
add_subdirectory(tests)

# TODO: Add tests and install targets if needed.
