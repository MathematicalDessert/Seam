#include "parse/Token.h"

namespace seam
{
	Token::Token(const TokenType type)
		: type_(type) {}

	Token::Token(const TokenType type, std::string data)
		: type_(type), data_(std::move(data)) {}

	bool Token::is_of_type(const TokenType type) const {		
		return this->type_ == type;
	}

	const std::string& Token::get_data() const {
		return data_;
	}
}