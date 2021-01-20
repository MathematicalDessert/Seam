# Seam Compiler Core

# Find LLVM Installation
find_package(LLVM CONFIG REQUIRED)

# Include LLVM includes and definitions
include_directories(include)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_library(seam "src/parse/Token.cpp"  "src/parse/Lexer.cpp" "src/parse/LexState.cpp" "src/core/Module.cpp" "src/core/ModuleManager.cpp")

install(TARGETS seam
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib)