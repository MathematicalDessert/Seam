#include "lexer.h"
#include <cwctype>

#include "exception.h"
#include "localisation/localisation.h"

namespace seam {
	Lexer::Lexer(const Source* source)
		: source_reader_(source) {}

	wchar_t Lexer::peek_character(const size_t num_characters_ahead) const {
		return source_reader_.peek_char(num_characters_ahead);
	}

	wchar_t Lexer::next_character() {
		return source_reader_.next_char();
	}

	std::wstring Lexer::consume() {
		return source_reader_.consume();
	}

	SourcePosition Lexer::get_current_pos() const {
		return SourcePosition{
			current_start_idx_,
			source_reader_.current_pos(),
		};
	}
	
	void Lexer::expect(const wchar_t character) {
		if (character != peek_character()) {
			// throw exception
		}
		source_reader_.discard(1);
	}

	std::wstring Lexer::read_until(const wchar_t character) {
		auto current_character = peek_character();

		while (current_character != character) {
			if (current_character == WEOF) {
				throw UnexpectedEOFException(
					get_current_pos(),
					LEX_UNEXPECTED_WEOF_EXCEPTION_FMT,
					character);
			}
			
			next_character();
			current_character = peek_character();
		}

		current_end_idx_ = source_reader_.current_pos(); // add + 1 for closing tag
		auto lexeme = consume();
		source_reader_.discard(1);

		return lexeme;
	}

	void Lexer::lex_comment() {
		const auto is_long_comment = peek_character() == '/';

		if (is_long_comment) {
			source_reader_.discard(1);
		}

		while (true) {
			if (is_long_comment) {
				if (peek_character() == '/'
					&& peek_character(1) == '/'
					&& peek_character(2) == '/') {

					const auto comment = consume();
					source_reader_.discard(3);

					break;
				}

				if (next_character() == WEOF) {
					throw UnexpectedEOFException(
						get_current_pos(),
						LEX_UNEXPECTED_WEOF_EXCEPTION_FMT,
						L"///");
				}
				// TODO: terminate on 3 slashes
			}
			else if (const auto next_wchar_t = next_character(); next_wchar_t == '\n' || next_wchar_t == WEOF) {
				break;
			}
		}

		source_reader_.discard();
	}
	
	void Lexer::tokenize_number_literal() {
		
	}
	
	void Lexer::tokenize_symbol() {
		SymbolType symbol;
		
		switch (next_character()) {
		case '+': symbol = SymbolType::PlusSign; break;
		case '-': symbol = SymbolType::MinusSign; break;
		case '/': symbol = SymbolType::ForwardSlash; break;
		default: break;
		}

		next_token_ = std::make_unique<Token>(
			symbol,
			nullptr,
			SourcePosition{
				current_start_idx_,
				current_end_idx_
			});
	}
	
	void Lexer::tokenize_identifier_or_keyword() {
		next_token_ = std::make_unique<Token>(
			SymbolType::None,
			L"",
			SourcePosition{ current_start_idx_, source_reader_.read_pointer() });
	}
		
	void Lexer::tokenize_string() {
		// opening tag
		expect('"');

		// read until closing tag
		auto lexeme = read_until('"');

		next_token_ = std::make_unique<Token>(
			SymbolType::StringLiteral,
			std::move(lexeme),
			SourcePosition {
				current_start_idx_,
				current_end_idx_
			});
	}
	
	void Lexer::tokenize() {
		source_reader_.discard_whitespace();

		// set start read index
		current_start_idx_ = source_reader_.start_pointer();

		if (const auto next_character = peek_character(); next_character == '"') {
			tokenize_string();
		} else if (next_character == '/' && peek_character(1) == '/') {
			source_reader_.discard(2);
			lex_comment();
			tokenize();
		} else if (std::iswdigit(next_character)) {
			tokenize_number_literal();
		} else if (std::iswpunct(next_character)) {
			tokenize_symbol();
		} else {
			tokenize_identifier_or_keyword();
		}
	}

	SymbolType Lexer::peek() {
		if (!next_token_) {
			tokenize();
		}
		
		return next_token_->type;
	}

	std::unique_ptr<Token> Lexer::next() {
		if (!next_token_) {
			tokenize();
		}
		
		return std::move(next_token_);
	}
}
