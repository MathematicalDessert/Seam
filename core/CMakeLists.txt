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
				 src/code_generation/code_generator.cpp 
				 src/parser/passes/type_analyzer.cpp
				 src/types/type.cpp
				 src/types/number_type.cpp "src/types/base_type.cpp" "src/types/void_type.cpp" "src/types/bool_type.cpp" "src/types/string_type.cpp" "src/types/class_type.cpp" "src/types/alias_type.cpp")

install(TARGETS core
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib)