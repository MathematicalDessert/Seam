# Seam Compiler Executable

# Find LLVM Installation
find_package(LLVM CONFIG REQUIRED)

add_executable(compiler src/compiler.cpp)
include_directories(${CMAKE_SOURCE_DIR}/core/src ${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(LLVM_LIBS support core irreader ${LLVM_TARGETS_TO_BUILD})

target_link_libraries(compiler core ${LLVM_LIBS})


install(TARGETS compiler RUNTIME DESTINATION bin)