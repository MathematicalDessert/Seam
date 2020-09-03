# Seam Compiler Core

add_library(core src/lexer/lexer.cpp src/parser/parser.cpp)

install(TARGETS core
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib)