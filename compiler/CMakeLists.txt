# Seam Compiler Executable

find_package(LLVM CONFIG REQUIRED)

add_executable(compiler src/Compiler.cpp)
include_directories(${CMAKE_SOURCE_DIR}/seam/include ${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

llvm_map_components_to_libnames(LLVM_LIBS support core irreader ${LLVM_TARGETS_TO_BUILD})

target_link_libraries(compiler seam ${LLVM_LIBS})

install(TARGETS compiler RUNTIME DESTINATION bin)