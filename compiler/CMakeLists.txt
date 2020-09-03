# Seam Compiler Executable

add_executable(compiler src/compiler.cpp)
include_directories(${CMAKE_SOURCE_DIR}/core/src)
target_link_libraries(compiler core)

install(TARGETS compiler RUNTIME DESTINATION bin)