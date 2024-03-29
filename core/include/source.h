#pragma once

#include <string>

namespace seam {
	// contains source
	class Source {
		std::wstring string_src_;
	public:
		explicit Source(std::wstring source);
		
		[[nodiscard]] const std::wstring& get() const { return string_src_; }

		friend class SourceReader;
	};

	// contains reference to source object
	// and reads accordingly
	class SourceReader {
		size_t start_pointer_ = 0;
		size_t read_pointer_ = 0;

		std::wstring buffer_;
		
		const Source* source_;
	public:
		explicit SourceReader(const Source* source);

		[[nodiscard]] size_t length() const { return source_->string_src_.length(); }

		// character read methods
		[[nodiscard]] wchar_t get_char(size_t pos) const;
		[[nodiscard]] wchar_t peek_char(size_t num_chars_ahead = 0) const;
		[[nodiscard]] wchar_t next_char();

		[[nodiscard]] std::wstring consume();
		
		void discard();
		void discard(size_t num_characters);
		void discard_whitespace();
		
		// pointer read methods
		[[nodiscard]] auto start_pointer() const { return start_pointer_; }
		[[nodiscard]] auto read_pointer() const { return read_pointer_; }
		[[nodiscard]] auto current_pos() const { return start_pointer_ + read_pointer_; }
	};
}
