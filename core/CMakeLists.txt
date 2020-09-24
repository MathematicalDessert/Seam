# Seam Compiler Core

# Find LLVM Installation
find_package(LLVM CONFIG REQUIRED)

# Include LLVM includes and definitions
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_library(core src/lexer/lexer.cpp 
				 src/parser/parser.cpp
				 src/ir/ast/statement.cpp
				 src/ir/ast/expression.cpp
				 src/debug/graphviz_visitor.cpp
				 src/code_generation/code_generator.cpp "src/parser/passes/type_analyzer.cpp")

install(TARGETS core
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib)