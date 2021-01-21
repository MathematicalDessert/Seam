#pragma once

#include <string>

namespace seam {
	enum class TokenType {
		tkEOF, // EOF
		tkIdentifier, // <identifier>
		tkAttribute, // @attribute
		tkComment, // comment

		tkStringLiteral, // literal
		tkIntegerLiteral, // integer literal
		tkFloatLiteral, // float literal

		// keywords
		kwLet,
		kwFunction,
		kwType,
		kwReturn,

		// symbols
		symbArrow,
		symbOpenBrace,
		symbCloseBrace,
		symbOpenBracket,
		symbCloseBracket,
		symbSeparator,
		symbColon,
		symbEqual,
		symbColonEqual,
	};
	
	class Token {
		friend class Lexer;
		
		// flavor of token
		TokenType type_;

		// token data
		std::string data_;
	public:
		explicit Token(TokenType type);
		explicit Token(TokenType type, std::string data);
		
		/**
		 * Returns token type as a string.
		 * 
		 * @param type type of token.
		 * @returns token type as a string.
		 */
		static std::string token_type_to_string(TokenType type);
		
		std::string type_as_string();
		std::string to_string();

		[[nodiscard]] bool is_of_type(TokenType type) const;
		[[nodiscard]] const std::string& get_data() const;
	};
}