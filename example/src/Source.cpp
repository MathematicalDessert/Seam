#include <iostream>
#include <parse/LexState.h>
#include "parse/Lexer.h"

int main() {
	auto source = seam::LexState(R"(
		@constructor
		fn main(arg: i64) -> i64 {
			let a = "asd"
		}
)");
	auto lexer = seam::Lexer(source);

	try {
		while (lexer.next() != seam::TokenType::tkEOF) {
			std::cout << lexer.token().to_string() << " ";
		}
		std::cout << std::endl;
	} catch (seam::LexicalException& e) {
		std::cout << e.get_position().line << ":" << e.get_position().column << ":";
		std::cout << e.what() << std::endl;
	}

	return 0;
}
