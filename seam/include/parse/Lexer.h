#pragma once

#include <memory>

#include "LexState.h"
#include "Token.h"

namespace seam {
	class Lexer {
		LexState& state_;

		Token current_token_;
		TokenType peeked_token_type_ = TokenType::tkEOF;
		TokenType current_token_type_ = TokenType::tkEOF;

		static bool is_identifier_character(char identifier_character);
		static bool is_start_identifier_character(char identifier_character);
		
		void lex_identifier();
		void lex_comment();
		void lex();
	public:
		explicit Lexer(LexState& source);

		/**
		 * 
		 *
		 * @returns type of next token.
		 */
		TokenType peek();
		
		/**
		 *
		 *
		 * @returns 
		 */
		TokenType next();

		/**
		 * 
		 */
		Token& token();
	};
}