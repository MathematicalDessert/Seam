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
		
		[[nodiscard]] bool is_of_type(TokenType type) const;
		[[nodiscard]] const std::string& get_data() const;
	};
}
