#include <utility>
#include <cmath>

#include "parse/LexState.h"

namespace seam {
	LexState::LexState(std::string source)
		: source_(std::move(source)) {
		lines_.push_back(0);
	}

	char LexState::peek_character(const int distance) {
		const auto character_position = current_read_start_ + current_read_offset_ + distance;

		if (character_position >= source_.size()) {
			return EOF;
		}

		return source_.at(character_position);
	}

	void LexState::skip_character() {
		// TODO: Test skip character works as expected.
		current_read_start_ += current_read_offset_ + 1;
		current_read_offset_ = 0;
	}

	void LexState::step_back() {
		if (current_read_offset_ > 0) {
			current_read_offset_--;
		} else if (current_read_start_ > 0) {
			current_read_start_--;
		}
	}

	
	void LexState::next_character() {
		const auto character_position = current_read_start_ + current_read_offset_++;
		const auto character = source_.at(character_position);

		if (character == '\n') { // if new line, increase line number, set column to one
			// TODO: Check new lines vector
			lines_.push_back(current_read_start_ + current_read_offset_ + 1); // add new line start position
		}

		num_characters_read_ += 1;
	}

	std::string LexState::consume() {
		auto data = source_.substr(current_read_start_, current_read_offset_);
		discard();

		return data;
	}

	void LexState::discard() {
		current_read_start_ = current_read_start_ + current_read_offset_;
		current_read_offset_ = 0;
	}

	// TODO: Write test for this!
	size_t LexState::get_line(const size_t character_position) {
		// we can't for certain give a line number, if we have not reached
		// that line yet.
		if (character_position > num_characters_read_) {
			return 0;
		}

		// prevents two calls to .size()
		const auto size_of_lines = lines_.size();

		// modified binary search
		auto lhs = 0;
		auto rhs = static_cast<int>(size_of_lines - 1);

		while (lhs <= rhs) {
			// size_t instead of int for consistency
			const auto search_position = static_cast<size_t>(std::floor(lhs + (rhs - lhs) / 2)); // mid point
			const auto selected_character_position = lines_[search_position]; // value of midpoint

			// case 1: by chance, it's the first character on a line
			if (selected_character_position == character_position) {
				return search_position + 1; // array index + 1 == line (arrays start at 0)
			}

			// case 2: position is greater than start of current line
			if (selected_character_position < character_position) {
				// upper bound: next line start position
				const auto upper_bound = search_position + 1;

				// check whether the next greatest element, if there is one,
				// is greater than this one - if so: this value is within bound
				// same goes for if this is the last element.
				if (upper_bound == size_of_lines
					|| upper_bound < size_of_lines && character_position < lines_[upper_bound]) {
					return search_position + 1;
				}

				lhs = search_position + 1;
			}
			else if (selected_character_position > character_position) {
				const auto lower_bound = search_position - 1;

				// check bounds
				if (lower_bound >= 0 && lines_[lower_bound] <= character_position) {
					return lower_bound + 1;
				}

				rhs = search_position - 1;
			}
		}

		return 0; // line not found
	}

	// TODO: Write test for this!
	size_t LexState::get_column(const size_t character_position) {
		const auto line_number = get_line(character_position);

		if (line_number == 0) {
			return 0;
		}
		
		const auto line = lines_[line_number];
		return character_position - line + 1;
	}

	SourcePosition LexState::get_line_and_column(const size_t character_position) {
		const auto line_number = get_line(character_position);

		if (line_number == 0) {
			return { 0, 0 };
		}

		return { line_number, character_position - lines_[line_number] + 1 };
	}
}
