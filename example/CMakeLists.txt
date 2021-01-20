# Seam Example Compiler Executable

# Find LLVM Installation
find_package(LLVM CONFIG REQUIRED)

add_executable(example src/Source.cpp)
include_directories(${CMAKE_SOURCE_DIR}/seam/include ${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(LLVM_LIBS support core irreader ${LLVM_TARGETS_TO_BUILD})

target_link_libraries(example seam ${LLVM_LIBS})

install(TARGETS example RUNTIME DESTINATION bin)