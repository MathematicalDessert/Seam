#include "source.h"

#include <utility>

namespace seam {
	// TODO: Add EOF!
	
	Source::Source(std::string source)
		: string_src_(std::move(source)) {
		
	}

	SourceReader::SourceReader(const Source* source)
		: source_(source) {
		
	}

	char SourceReader::get_char(const size_t pos) const {
		if (pos >= length()) {
			return EOF;
		}
		
		return source_->string_src_.at(pos);
	}

	char SourceReader::peek_char(const size_t num_chars_ahead) const {
		if (start_pointer_ + read_pointer_ + num_chars_ahead >= length()) {
			return EOF;
		}
		
		return source_->string_src_.at(start_pointer_ + read_pointer_);
	}
	
	char SourceReader::next_char() {
		if (start_pointer_ + read_pointer_ >= length()) {
			return EOF;
		}
		
		return source_->string_src_.at(start_pointer_ + read_pointer_++);
	}

	std::string SourceReader::consume() {
		auto new_str =  source_->string_src_.substr(start_pointer_, read_pointer_);

		discard();
		
		return new_str;
	}

	void SourceReader::discard() {
		discard(0);
	}
	
	void SourceReader::discard(const size_t num_characters) {
		start_pointer_ += read_pointer_ + num_characters;
		read_pointer_ = 0;
	}

	void SourceReader::discard_whitespace() {
		while (std::isspace(peek_char())) {
			discard(1);
		}
	}
}
