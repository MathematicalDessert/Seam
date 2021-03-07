#include "parse/Lexer.h"
#include "parse/Token.h"

#include <unordered_map>

namespace seam {
	const std::unordered_map<std::string, TokenType> keyword_map = {
		{ "let", TokenType::kwLet },
		{ "fn", TokenType::kwFunction },
		{ "type", TokenType::kwType },
	};
	
	const std::unordered_map<std::string, TokenType> symbol_map = {
		{ "->", TokenType::symbArrow },
		{ "{", TokenType::symbOpenBrace },
		{ "}", TokenType::symbCloseBrace },
		{ "(", TokenType::symbOpenBracket },
		{ ")", TokenType::symbCloseBracket },
		{ ";", TokenType::symbSeparator },
		{ ":", TokenType::symbColon },
		{ "=", TokenType::symbEqual },
		{ ":=", TokenType::symbColonEqual },
	};

	bool Lexer::is_identifier_character(const char identifier_character) {
		return identifier_character == '_'
				|| std::isalnum(identifier_character);
	}

	bool Lexer::is_start_identifier_character(const char identifier_character) {
		return identifier_character == '_'
				|| std::isalpha(identifier_character);
	}

	void Lexer::lex_symbol(Token& token) {
		auto counter = 0;
		auto character = state_.peek_character(counter);
		
		std::string symbol_to_search = std::string { character };

		character = state_.peek_character(++counter);
		while (std::ispunct(character)) {
			symbol_to_search = symbol_to_search + character;
			character = state_.peek_character(++counter);
		}

		// TODO: Could optimise by knowing longest possible

		for (auto i = symbol_to_search.length(); i > 0; i--) {
			if (auto iterator = symbol_map.find(symbol_to_search.substr(0, i)); iterator != symbol_map.cend()) {
				for (size_t remove_count = 0; remove_count < i; remove_count++) {
					state_.skip_character();
				}
				token.type_ = iterator->second;
				return; // terminate search & resolve
			}
		}
		
		throw LexicalException("unknown symbol found", state_.get_current_line_and_column());
	}

	void Lexer::lex_number(Token& token) {
		auto is_hex = false;
		auto is_float = false;

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
				break;
			}

			state_.next_character();
			peeked_character = state_.peek_character();
		}
		
		token.data_ = state_.consume();
		token.type_ = is_float ? TokenType::tkFloatLiteral : TokenType::tkIntegerLiteral;
	}
	
	std::string Lexer::lex_identifier(Token& token) {
		if (!is_start_identifier_character(state_.peek_character())) {
			throw LexicalException("invalid identifier", state_.get_current_line_and_column());
		}
		state_.next_character();
		
		while (is_identifier_character(state_.peek_character())) {
			state_.next_character();
		}

		return state_.consume();
	}

	void Lexer::lex_identifier_or_keyword(Token& token) {
		const auto identifier = lex_identifier(token);

		if (auto iterator = keyword_map.find(identifier); iterator != keyword_map.cend()) {
			token.type_ = iterator->second;
		} else {
			token.type_ = TokenType::tkIdentifier;
			token.data_ = identifier;
		}
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
		switch(state_.peek_character(0, true)) {
		case EOF: { // EOF
			state_.skip_character();
			token.type_ = TokenType::tkEOF;
			break;
		}
		case '@': { // Attribute
			state_.skip_character(); // skip @ character
			token.data_ = lex_identifier(token); // lex attribute name
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
				lex_identifier_or_keyword(token);
			} else if (std::isdigit(peeked_character)) { // number literal
				lex_number(token);
			} else if (std::ispunct(peeked_character)) { // symbol
				lex_symbol(token);
			} else {
				throw LexicalException(string_format("unknown character found in stream: '%s'\n", peeked_character), state_.get_current_line_and_column());
				// TODO: throw unknown character!
			}
			break;
		}
		}
	}
	
	Lexer::Lexer(LexState& source)
		: state_(source), peeked_token_(TokenType::tkEOF), current_token_(TokenType::tkEOF) {}

	TokenType Lexer::peek() {
		if (peeked_token_.type_ == TokenType::tkEOF) {
			peeked_token_ = Token(TokenType::tkEOF);
			lex(peeked_token_);
		}
		
		return peeked_token_.type_;
	}

	TokenType Lexer::next() {
		if (peeked_token_.type_ != TokenType::tkEOF) {
			current_token_ = peeked_token_;
			peeked_token_ = Token(TokenType::tkEOF);
		} else {
			current_token_ = Token(TokenType::tkEOF);
			lex(current_token_);
		}
		
		return current_token_.type_;
	}

	Token& Lexer::token() {
		return current_token_;
	}
}
