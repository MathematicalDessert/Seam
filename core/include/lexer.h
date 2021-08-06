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
		const std::wstring lexeme;
		// token position
		const SourcePosition position;

		Token(const SymbolType type, std::wstring lexeme, const SourcePosition position)
			: type(type), lexeme(std::move(lexeme)), position(position) {}
	};

	/**
	 * Lexical Analysis Class.
	 *
	 * Takes in a Source and generates tokens.
	 */
	class Lexer {
		// Take in a string
		// Tokenize string

		// reference to source
		SourceReader source_reader_;
		
		[[nodiscard]] wchar_t peek_character(size_t num_characters_ahead = 0) const;
		wchar_t next_character();
		std::wstring consume();

		[[nodiscard]] SourcePosition get_current_pos() const;
		
		void expect(wchar_t character);
		std::wstring read_until(wchar_t character);

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
