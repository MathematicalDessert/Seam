#pragma once

#include <string>
#include <vector>

#include "core/Exception.h"

namespace seam {
	/**
	 * The LexState class contains a state that is used by the lexer to
	 * traverse through a source.
	 */
	class LexState {
		// reference to source
		std::string source_;

		// current read start index
		size_t current_read_start_ = 0;

		// current read offset
		size_t current_read_offset_ = 0;

		// distance we've seeked
		size_t num_characters_read_ = 0;

		// start lines
		std::vector<size_t> lines_;
	public:
		/**
		 * Default constructor.
		 *
		 * @param source string to lex.
		 */
		explicit LexState(std::string source);

		/**
		 * Returns next character.
		 *
		 * @param distance distance from next immediate character to return.
		 * @returns next character.
		 */
		[[nodiscard]] char peek_character(int distance = 0);

		/**
		 * Resets collected characters, and skips current.
		 */
		void skip_character();

		/**
		 * Steps back a character.
		 */
		void step_back();
		
		/**
		 * Collects current character.
		 */
		void next_character();

		/**
		 * Returns currently collected characters.
		 *
		 * @returns string containing characters currently collected.
		 */
		[[nodiscard]] std::string consume();

		/**
		 * Discards string of characters currently collected through
		 * next_character.
		 */
		void discard();

		/**
		 * TODO: Comment.
		 */
		bool should_save_comments();

		/**
		 * Returns line position of a character from its index in
		 * source string.
		 *
		 * @param character_position position of character in source.
		 * @returns line index.
		 */
		[[nodiscard]] size_t get_line(size_t character_position);
		
		/**
		 * Returns column position of a character from its index in
		 * source string.
		 *
		 * @param character_position position of character in source.
		 * @returns column index.
		 */
		[[nodiscard]] size_t get_column(size_t character_position);

		/**
		 * Returns line & column of where character position is.
		 *
		 * @param character_position position of character in source.
		 * @returns SourcePosition containing line and column of character position.
		 */
		[[nodiscard]] SourcePosition get_line_and_column(size_t character_position);

		/**
		 * Returns current line & column.
		 * 
		 * @returns SourcePosition containing current line and column position.
		 */
		[[nodiscard]] SourcePosition get_current_line_and_column();
	};
}
