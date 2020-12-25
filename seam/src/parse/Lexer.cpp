#include "parse/Lexer.h"
#include "parse/Token.h"

namespace seam {
	bool Lexer::is_identifier_character(const char identifier_character) {
		return identifier_character == '_'
				|| std::isalnum(identifier_character);
	}

	bool Lexer::is_start_identifier_character(const char identifier_character) {
		return identifier_character == '_'
				|| std::isalpha(identifier_character);
	}

	
	void Lexer::lex_identifier() {
		if (!is_start_identifier_character(state_.peek_character())) {
			return; // TODO: Add exception for invalid identifier
		}
		state_.next_character();
		
		while (is_identifier_character(state_.peek_character())) {
			state_.next_character();
		}
		current_token_.data_ = state_.consume();
	}

	void Lexer::lex_comment() {
		const auto is_long_comment = state_.peek_character() == '/';

		if (is_long_comment) {
			state_.next_character();
		}

		state_.discard(); // discard comment opening

		
		while (true) {
			const auto current_character = state_.peek_character();

			if (is_long_comment && state_.peek_character(1) == '/' && state_.peek_character(2)) {
				break;
			}
			
			if (!is_long_comment && current_character == '\n') {
				break;
			}

			state_.next_character();
		}
		
		if (state_.should_save_comments()) {
			current_token_.data_ = state_.consume();
		} else {
			state_.discard();
		}

		if (is_long_comment) {
			state_.next_character();
			state_.next_character();
			state_.next_character();
		} else {
			state_.next_character();
		}
		state_.discard();
	}

	
	void Lexer::lex() {
		switch(state_.peek_character()) {
		case EOF: { // EOF
			state_.skip_character();
			current_token_.type_ = TokenType::tkEOF;
			break;
		}
		case '@': { // Attribute
			state_.skip_character(); // skip @ character
			lex_identifier(); // lex attribute name
			current_token_.type_ = TokenType::tkAttribute;
			break;
		}
		case '"': { // String Literal
			break;
		}
		case '/': { // Comment
			state_.skip_character();

			if (state_.peek_character() == '/') { // is comment
				state_.skip_character();
				lex_comment();
				break;
			}

			// otherwise is operator?
			[[fallthrough]];
		}
		default: { // Operators, Keywords, and Identifiers
			current_token_.type_ = TokenType::tkEOF;
			break;
		}
		}
	}
	
	Lexer::Lexer(LexState& source)
		: state_(source), current_token_(TokenType::tkEOF) {}

	TokenType Lexer::peek() {
		// TODO: Write peek token
		
		return peeked_token_type_;
	}

	TokenType Lexer::next() {
		// TODO: Write next token
		
		return current_token_type_;
	}

	Token& Lexer::token() {
		// lex_token() ???
		return current_token_;
	}
}
