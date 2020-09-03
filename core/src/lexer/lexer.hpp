#pragma once

#include <optional>

#include "lexeme.hpp"
#include "../utils/position.hpp"

namespace seam::core::lexer
{
	/**
	 * Lexical analysis class.
	 *
	 * Takes in a source and returns valid lexemes.
	 */
	class lexer
	{
		constexpr static char eof_character = -1;
		
		std::string_view source_;
		std::optional<lexeme> current_lexeme_;
		std::optional<lexeme> peeked_lexeme_;

		std::size_t read_offset_ = 0;
		std::size_t line_ = 1;
		std::size_t line_start_offset_ = 0;

		[[nodiscard]] utils::position current_position() const;
		[[nodiscard]] char peek_character(std::size_t offset = 0) const;
		void consume_character();

		void skip_whitespace();
		void skip_comment();
		void lex_string_literal(lexeme& ref);
		void lex_number_literal(lexeme& ref);
		void lex_keyword_or_identifier(lexeme& ref);
		void lex_attribute(lexeme& ref);
		void lex_symbol(lexeme& ref);
		
		void lex(lexeme& ref);
	public:
		explicit lexer(const std::string_view& source);

		lexeme_type peek_lexeme();
		lexeme& next_lexeme();
	};
}
