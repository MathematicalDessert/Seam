# Find LLVM Installation
#find_package(LLVM CONFIG REQUIRED)

# Include LLVM includes and definitions
include_directories(include)

#include_directories(${LLVM_INCLUDE_DIRS})
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
#include(HandleLLVMOptions)
add_definitions(${LLVM_DEFINITIONS})

add_library(seam 
			"src/lexer.cpp" "src/source.cpp" "include/source_position.h" "include/exception.h" "include/localisation/en_gb.h" "include/localisation/localisation.h" )

target_include_directories(seam PRIVATE ${LLVM_INCLUDE_DIRS})
#llvm_map_components_to_libnames(llvm_libs Support Core IRReader)

target_link_libraries(seam PRIVATE ${llvm_libs})

#install(TARGETS seam
#		LIBRARY DESTINATION lib
#		ARCHIVE DESTINATION lib)