#pragma once

#include <codecvt>
#include <exception>
#include <stdexcept>
//#include <format>
#include <utility>
#include <fmt/format.h>
#include <locale>

#include "source_position.h"
#include <fmt/xchar.h>

namespace seam {
	template<class T, typename... Args>
	T generate_exception(const SourcePosition source_position, std::wstring exception_message, Args&&... args) {
		return T(source_position, fmt::format(exception_message.c_str(), args...));
	}

	class SeamException : public std::runtime_error {
		const SourcePosition pos_;
	protected:
		SeamException(const SourcePosition source_position, const std::wstring& exception_message)
			: std::runtime_error(std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().to_bytes(exception_message)),
			  pos_(source_position) {}
	public:
		SeamException(const std::wstring& exception_message)
			: std::runtime_error(std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().to_bytes(exception_message)),
			pos_({ 0, 0 }) {}
	public:
		[[nodiscard]] SourcePosition position() const { return pos_; }
	};
	
	class LexicalException final : public SeamException {
	public:
		LexicalException(const SourcePosition source_position, std::wstring exception_message)
			: SeamException(source_position, std::move(exception_message)) {}
	};

	class ParserException final : public SeamException {
	public:
		ParserException(const SourcePosition source_position, std::wstring exception_message)
			: SeamException(source_position, std::move(exception_message)) {}
	};
}
