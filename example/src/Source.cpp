#include <iostream>
#include <parse/LexState.h>
#include "parse/Lexer.h"

int main() {
	auto source = seam::LexState(R"(@1test-asd)");
	auto lexer = seam::Lexer(source);

	try {
		lexer.next();
	} catch (seam::LexicalException& e) {
		std::cout << e.get_position().line << ":" << e.get_position().column << ":";
		std::cout << e.what() << std::endl;
	}

	return 0;
}
