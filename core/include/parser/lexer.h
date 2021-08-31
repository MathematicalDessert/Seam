#pragma once

#include <unordered_map>
#include <memory>

#include "source.h"
#include "source_position.h"
#include "symbols.h"

namespace seam {
	/**
	 * Token Object.
	 *
	 * @note Immutable, this object should not be promoted
	 * or demoted in any way.
	 */
	struct Token {
		// token type
		const SymbolType type;
		// token lexeme
		const std::wstring lexeme;
		// token position
		const SourcePosition position;

		Token(const SymbolType type, std::wstring lexeme, const SourcePosition position)
			: type(type), lexeme(std::move(lexeme)), position(position) {}
	};

	/**
	 * Lexical Analysis Class.
	 *
	 * Takes in a Source and generates tokens.
	 */
	class Lexer {
		// reference to source
		SourceReader source_reader_;

		// current start index
		size_t current_start_idx_ = 0;

		// current end index
		size_t current_end_idx_ = 0;

		// next lex token.
		std::unique_ptr<Token> next_token_;

		/**
		 * Peeks a character n + 1 positions ahead. Default n = 0.
		 *
		 * @param num_characters_ahead number of characters ahead to peek.
		 *
		 * @returns peeked character.
		 *
		 * @note this does <b>NOT</b> step the state of the lexer.
		 */
		[[nodiscard]] wchar_t peek_character(size_t num_characters_ahead = 0) const;

		/**
		 * Returns the next character from the lexer.
		 *
		 * @returns next character.
		 *
		 * @note this steps the state of the lexer.
		 */
		wchar_t next_character();

		/**
		 * Consumes the current characters in the lexer and returns them
		 * in the form of a string.
		 *
		 * @returns string containing characters currently stored in buffer.
		 */
		std::wstring consume();

		/**
		 * Returns current start and end position.
		 *
		 * @returns current position in source.
		 */
		[[nodiscard]] SourcePosition get_current_pos() const;

		/**
		 * Reads a string until a character is found.
		 *
		 * @param character character to halt on.
		 *
		 * @returns read and consumed string.
		 */
		std::wstring read_until(wchar_t character);

		/**
		 * Returns symbol from map if peek character match is found,
		 * otherwise returns default symbol.
		 *
		 * @param map map of characters to symbols.
		 * @param default_symbol default symbol.
		 *
		 * @returns symbol type.
		 */
		SymbolType check_next(const std::unordered_map<wchar_t, SymbolType>& map, SymbolType default_symbol);

		/**
		 * Lex a comment.
		 */
		void tokenize_comment();

		/**
		 * Lex of a number literal.
		 */
		void tokenize_number_literal();

		/**
		 * Lex a symbol.
		 */
		void tokenize_symbol();

		/**
		 * Lex an identifier or keyword.
		 */
		void tokenize_identifier_or_keyword();

		/**
		 * Lex a string.
		 */
		void tokenize_string();

		/**
		 * Steps the lexer to lex the next token.
		 */
		void tokenize();		
	public:
		/**
		 * Constructor.
		 *
		 * @param source source object to lex.
		 */
		explicit Lexer(const Source* source);

		/**
		 * Peeks next token.
		 *
		 * @returns peeked token.
		 */
		[[nodiscard]] SymbolType peek();

		/**
		 * Returns next token.
		 *
		 * @returns next token.
		 */
		[[maybe_unused]] std::unique_ptr<Token> next();
	};
}
