#include "parse/Token.h"

namespace seam {
	Token::Token(const TokenType type)
		: type_(type) {}

	Token::Token(const TokenType type, std::string data)
		: type_(type), data_(std::move(data)) {}

	std::string Token::token_type_to_string(TokenType type) {
		switch (type) {
			case TokenType::tkEOF: return "<EOF>";
			case TokenType::tkIdentifier: return "<IDENTIFIER>";
			case TokenType::tkAttribute: return "<ATTRIBUTE>";
			case TokenType::tkComment: return "<COMMENT>";
			case TokenType::tkKeyword: return "<KEYWORD>";
			case TokenType::tkStringLiteral: return "<STRING>";
			case TokenType::tkIntegerLiteral: return "<INTEGER>";
			case TokenType::tkFloatLiteral: return "<FLOAT>";
			case TokenType::kwLet: return "<KW_LET>";
			case TokenType::kwFunction: return "<KW_FN>";
			case TokenType::symbArrow: return "->";
			case TokenType::symbOpenBrace: return "{";
			case TokenType::symbCloseBrace: return "}";
			case TokenType::symbOpenBracket: return "(";
			case TokenType::symbCloseBracket: return ")";
			case TokenType::symbColon: return ":";
			case TokenType::symbEqual: return "=";
			case TokenType::symbColonEqual: return ":=";
			default: {
				return "Unknown!";
			}
		}
	}

	std::string Token::type_as_string() {
		return token_type_to_string(type_);
	}

	std::string Token::to_string() {
		if (data_.empty()) {
			return type_as_string();
		}

		return std::string("(" + type_as_string() + ", \"" + data_ + "\")");
	}

	bool Token::is_of_type(const TokenType type) const {		
		return this->type_ == type;
	}

	const std::string& Token::get_data() const {
		return data_;
	}
}