#include <cwctype>
#include <unordered_map>

#include "parser/lexer.h"
#include "exception.h"
#include "localisation/localisation.h"

namespace seam {
	namespace {
		const std::unordered_map<std::wstring, SymbolType> str_to_keyword_map {
			{ L"let", SymbolType::KeywordLet },
			{ L"fn", SymbolType::KeywordFn },
			{ L"type", SymbolType::KeywordType },
			{ L"while", SymbolType::KeywordWhile },
			{ L"for", SymbolType::KeywordFor },
			{ L"true", SymbolType::KeywordTrue },
			{ L"false", SymbolType::KeywordFalse },
			{ L"import", SymbolType::KeywordImport },
			{ L"if", SymbolType::KeywordIf },
			{ L"else", SymbolType::KeywordElse },
			{ L"elseif", SymbolType::KeywordElseIf }
		};
	}

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

	std::wstring Lexer::read_until(const wchar_t character) {
		auto current_character = peek_character();

		while (current_character != character) {
			if (current_character == WEOF) {
				throw generate_exception<LexicalException>(
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

	SymbolType Lexer::check_next(const std::unordered_map<wchar_t, SymbolType>& map, SymbolType default_symbol) {
		if (map.find(peek_character()) != map.cend()) {
			return map.at(next_character());
		}
		return default_symbol;
	}

	void Lexer::tokenize_comment() {
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
					throw generate_exception<LexicalException>(
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
		const auto is_hex = peek_character() == '0' && peek_character(1) == 'x';
		auto is_float = !is_hex && peek_character() == '.';

		if (is_hex) {
			next_character();
			next_character();
		} else if (is_float) {
			next_character();
		}

		if (const auto next_char = next_character(); is_hex && !std::iswxdigit(next_char) || !is_hex && !std::iswdigit(next_char)) {
			throw generate_exception<LexicalException>(
				get_current_pos(),
				LEX_MALFORMED_NUMBER_LITERAL);
		}

		while(true) {
			const auto peeked_character = peek_character();
			if (std::iswspace(peeked_character) || peeked_character == WEOF) {
				break; // parse number
			}

			if (is_hex) {
				if (!std::iswxdigit(peeked_character)) {
					throw generate_exception<LexicalException>(
						get_current_pos(),
						EXPECTED_BUT_GOT,
						L"hex-digit",
						peeked_character
						);
				}
			} else {
				if (is_float && peeked_character == '.') {
					throw generate_exception<LexicalException>(
						get_current_pos(),
						LEX_MALFORMED_FLOATING_POINT_NUMBER_LITERAL,
						LEX_MALFORMED_FLOAT_TWO_POINTS);
				}

				if (!is_float && peeked_character == '.') {
					is_float = true;
				} else if (!std::iswdigit(peeked_character)) {
					if (std::iswspace(peeked_character) || peeked_character == WEOF) {
						break;
					}

					throw generate_exception<LexicalException>(
						get_current_pos(),
						EXPECTED_BUT_GOT,
						L"digit",
						peeked_character
						);
				}
			}
			next_character();
		}

		current_end_idx_ = source_reader_.current_pos();
		next_token_ = std::make_unique<Token>(
			SymbolType::NumberLiteral,
			consume(),
			SourcePosition{
				current_start_idx_,
				current_end_idx_ - 1
			});
	}
	
	void Lexer::tokenize_symbol() {
		SymbolType symbol;
		
		switch (next_character()) {
		case '+': {
			symbol = check_next({
				{ '+', SymbolType::OpIncrement },
				{ '=', SymbolType::OpAddEq }},
				SymbolType::OpAdd);
			break;
		}
		case '-': {
			symbol = check_next({
				{ '-', SymbolType::OpDecrement },
				{ '=', SymbolType::OpSubEq },
				{ '>', SymbolType::Arrow }},
				SymbolType::OpSub);
			break;
		}
		case '/': symbol = SymbolType::OpDiv; break;
		case '*': symbol = SymbolType::OpMul; break;
		case '(': symbol = SymbolType::SymbOpenParen; break;
		case ')': symbol = SymbolType::SymbCloseParen; break;
		case '{': symbol = SymbolType::SymbOpenBrace; break;
		case '}': symbol = SymbolType::SymbCloseBrace; break;
		case '=': {
			symbol = check_next({
				{ '=', SymbolType::OpEq } },
				SymbolType::OpAssign);
			break;
		}
		case ':': {
			symbol = check_next({
				{ '=', SymbolType::ColonEquals } },
				SymbolType::Colon);
			break;
		}
		default: {
			throw generate_exception<LexicalException>(
				get_current_pos(),
				L"unknown symbol found {}",
				L"");
		}
		}

		source_reader_.discard();
		next_token_ = std::make_unique<Token>(
			symbol,
			L"",
			SourcePosition{
				current_start_idx_,
				current_end_idx_
			});
	}
	
	void Lexer::tokenize_identifier_or_keyword() {
		bool must_be_identifier = false;

		auto next_char = peek_character();
		
		while (std::iswalnum(next_char) || next_char == '_') {
			if (!must_be_identifier && next_char == '_') {
				must_be_identifier = true;
			}

			next_character();
			next_char = peek_character();
		}

		current_end_idx_ = source_reader_.current_pos();

		if (const auto identifier = consume(); !must_be_identifier && str_to_keyword_map.find(identifier) != str_to_keyword_map.cend()) {
			next_token_ = std::make_unique<Token>(
				str_to_keyword_map.at(identifier),
				L"",
				SourcePosition{ current_start_idx_, current_end_idx_ - 1 });
		} else {
			next_token_ = std::make_unique<Token>(
				SymbolType::Identifier,
				identifier,
				SourcePosition{ current_start_idx_, current_end_idx_ - 1 });
		}
	}
		
	void Lexer::tokenize_string() {
		// opening tag
		source_reader_.discard(1);

		// read until closing tag
		auto lexeme = read_until('"');

		next_token_ = std::make_unique<Token>(
			SymbolType::StringLiteral,
			std::move(lexeme),
			SourcePosition {
				current_start_idx_,
				current_end_idx_ - 1
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
			tokenize_comment();
			tokenize();
		} else if (std::iswdigit(next_character)
			|| next_character == L'.' && std::iswdigit(peek_character(1))) {
			tokenize_number_literal();
		} else if (std::iswalpha(next_character) || next_character == '_') {
			tokenize_identifier_or_keyword();
		} else if (std::iswpunct(next_character)) {
			tokenize_symbol();
		} 

		if (!next_token_) {
			// empty?
			next_token_ = std::make_unique<Token>(
				SymbolType::None,
				L"",
				SourcePosition{
					0, 0
				});
		}
	}

	Lexer::Lexer(const Source* source)
		: source_reader_(source) {}

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
