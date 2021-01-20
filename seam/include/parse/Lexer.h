#pragma once

#include "LexState.h"
#include "Token.h"

namespace seam {
	class Lexer {
		LexState& state_;

		Token peeked_token_;
		Token current_token_;
		TokenType peeked_token_type_ = TokenType::tkEOF;
		TokenType current_token_type_ = TokenType::tkEOF;

		static bool is_identifier_character(char identifier_character);
		static bool is_start_identifier_character(char identifier_character);
		
		void lex_number(Token& token);
		void lex_identifier(Token& token);
		void lex_comment(Token& token);
		void lex_string(Token& token);
		void lex(Token& token);
	public:
		explicit Lexer(LexState& source);

		/**
		 * Peeks and returns the type of the token.
		 *
		 * @returns type of peeked token.
		 */
		TokenType peek();
		
		/**
		 * Returns the type of the next token.
		 *
		 * @returns type of next token.
		 */
		TokenType next();

		/**
		 * Returns token value.
		 * 
		 * @returns token value.
		 */
		Token& token();
	};
}