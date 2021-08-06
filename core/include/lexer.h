#pragma once

#include <memory>

#include "source.h"
#include "source_position.h"
#include "symbols.h"

namespace seam {
	/**
	 * Token Object.
	 *
	 * @note Immutable, this object should not be promoted
	 * or demoted in any way.
	 */
	struct Token {
		// token type
		const SymbolType type;
		// token lexeme
		const std::string lexeme;
		// token position
		const SourcePosition position;

		Token(const SymbolType type, std::string lexeme, const SourcePosition position)
			: type(type), lexeme(std::move(lexeme)), position(position) {}
	};
	
	class Lexer {
		// Take in a string
		// Tokenize string

		// reference to source
		SourceReader source_reader_;
		
		[[nodiscard]] char peek_character(size_t num_characters_ahead = 0) const;
		char next_character();
		std::string consume();

		[[nodiscard]] SourcePosition get_current_pos() const;
		
		void expect(char character);
		std::string read_until(char character);

		void lex_comment();
		
		void tokenize_number_literal();
		void tokenize_symbol();
		void tokenize_identifier_or_keyword();
		void tokenize_string();

		void tokenize();
		
		size_t current_start_idx_ = 0;
		size_t current_end_idx_ = 0;
		
		std::unique_ptr<Token> next_token_;
	public:
		explicit Lexer(const Source* source);

		[[nodiscard]] SymbolType peek();
		std::unique_ptr<Token> next();
	};
}
