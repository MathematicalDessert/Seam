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

	void Lexer::lex_number(Token& token) {
		bool is_hex = false;
		bool is_float = false;

		auto peeked_character = state_.peek_character();
		while (true) {
			if (peeked_character == 'x') {
				if (is_float) {
					throw LexicalException("invalid float literal (found 'x')", state_.get_current_line_and_column());
				}

				if (is_hex) {
					throw LexicalException("invalid integer literal (second 'x' found)", state_.get_current_line_and_column());
				}

				is_hex = true;
			}

			if (peeked_character == '.') {
				if (is_float) {
					throw LexicalException("invalid float literal (second '.' found)", state_.get_current_line_and_column());
				}

				if (is_hex) {
					throw LexicalException("invalid integer literal (found 'x')", state_.get_current_line_and_column());
				}

				is_float = true;
			}

			if (is_hex && !std::isxdigit(peeked_character)) {
				throw LexicalException("non-xdigit character found in xdigit", state_.get_current_line_and_column());
			} else if (!std::isdigit(peeked_character)) {
				throw LexicalException("invalid number", state_.get_current_line_and_column());
			}

			state_.next_character();
			peeked_character = state_.peek_character();
		}
		
		token.data_ = state_.consume();
		token.type_ = is_float ? TokenType::tkFloatLiteral : TokenType::tkIntegerLiteral;
	}
	
	void Lexer::lex_identifier(Token& token) {
		if (!is_start_identifier_character(state_.peek_character())) {
			throw LexicalException("invalid identifier", state_.get_current_line_and_column());
		}
		state_.next_character();
		
		while (is_identifier_character(state_.peek_character())) {
			state_.next_character();
		}
		token.data_ = state_.consume();
	}

	void Lexer::lex_comment(Token& token) {
		const auto is_long_comment = state_.peek_character() == '/';

		if (is_long_comment) {
			state_.next_character();
		}

		state_.discard(); // discard comment opening
		
		while (true) {
			const auto current_character = state_.peek_character();

			if (is_long_comment && state_.peek_character(1) == '/' && state_.peek_character(2) == '/') {
				break;
			}
			
			if (!is_long_comment && (current_character == '\n' || current_character == EOF)) {
				break;
			}

			state_.next_character();
		}
		
		if (state_.should_save_comments()) {
			token.data_ = state_.consume();
		} else {
			state_.discard();
		}

		if (is_long_comment) {
			state_.next_character();
			state_.next_character();
			state_.next_character();
		} else {
			if (state_.peek_character() != EOF) {
				state_.next_character();
			}
		}
		state_.discard();
	}

	void Lexer::lex_string(Token& token) {
		while (state_.peek_character() != '"') {
			// TODO: add exception for EOF, unterminated string literal
			state_.next_character();
		}

		token.data_ = state_.consume();
		state_.skip_character(); // Skips closing "
	}
	
	void Lexer::lex(Token& token) {
		switch(state_.peek_character()) {
		case EOF: { // EOF
			state_.skip_character();
			token.type_ = TokenType::tkEOF;
			break;
		}
		case '@': { // Attribute
			state_.skip_character(); // skip @ character
			lex_identifier(token); // lex attribute name
			token.type_ = TokenType::tkAttribute;
			break;
		}
		case '"': { // String Literal
			state_.skip_character(); // skip opening "
			lex_string(token);
			token.type_ = TokenType::tkStringLiteral;
			break;
		}
		case '/': { // Comment
			state_.skip_character();

			if (state_.peek_character() == '/') { // is comment
				state_.skip_character();
				lex_comment(token);
				
				if (state_.should_save_comments()) {
					token.type_ = TokenType::tkComment;
				} else {
					lex(token); // find next token
				}
				
				break;
			}

			// otherwise is operator?
			[[fallthrough]];
		}
		default: { // Operators, Keywords, and Identifiers
			auto peeked_character = state_.peek_character();

			if (is_start_identifier_character(peeked_character)) { // identifier or keyword

			} else if (std::isdigit(peeked_character)) { // number literal
				lex_number(token);
			} else { // symbol

			}
			break;
		}
		}
	}
	
	Lexer::Lexer(LexState& source)
		: state_(source), peeked_token_(TokenType::tkEOF), current_token_(TokenType::tkEOF) {}

	TokenType Lexer::peek() {
		lex(peeked_token_);
		
		return peeked_token_.type_;
	}

	TokenType Lexer::next() {
		if (peeked_token_.type_ != TokenType::tkEOF) {
			current_token_ = peeked_token_;
			peeked_token_ = Token(TokenType::tkEOF);
		} else {
			lex(current_token_);
		}
		
		return current_token_.type_;
	}

	Token& Lexer::token() {
		return current_token_;
	}
}
