#include "source.h"

#include <cwctype>
#include <utility>

namespace seam {
	// TODO: Add WEOF!
	
	Source::Source(std::wstring source)
		: string_src_(std::move(source)) {
		
	}

	SourceReader::SourceReader(const Source* source)
		: source_(source) {
		
	}

	// TODO: write test!
	wchar_t SourceReader::get_char(const size_t pos) const {
		if (pos >= length()) {
			return WEOF;
		}
		
		return source_->string_src_.at(pos);
	}

	wchar_t SourceReader::peek_char(const size_t num_chars_ahead) const {
		if (start_pointer_ + read_pointer_ + num_chars_ahead >= length()) {
			return WEOF;
		}
		
		return source_->string_src_.at(start_pointer_ + read_pointer_ + num_chars_ahead);
	}
	
	wchar_t SourceReader::next_char() {
		if (start_pointer_ + read_pointer_ >= length()) {
			return WEOF;
		}
		
		return source_->string_src_.at(start_pointer_ + read_pointer_++);
	}

	std::wstring SourceReader::consume() {
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
		while (std::iswspace(peek_char())) {
			discard(1);
		}
	}
}
