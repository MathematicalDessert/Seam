#pragma once

#include <codecvt>
#include <exception>
#include <stdexcept>
#include <format>

#include "source_position.h"

namespace seam {
	class SeamExceptionBase : public std::runtime_error {
		const SourcePosition pos_;
	public:
		SeamExceptionBase(const SourcePosition source_position, const std::wstring& exception_message)
			: std::runtime_error(std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().to_bytes(exception_message)),
			  pos_(source_position) {}
	public:
		[[nodiscard]] SourcePosition position() const { return pos_; }
	};

	template<typename... Args>
	class SeamException : public SeamExceptionBase {
	protected:
		SeamException(const SourcePosition source_position, std::wstring exception_message, Args... args)
			: SeamExceptionBase(source_position, std::format(std::move(exception_message), args...)) {}
	};

	template<typename... Args>
	class UnexpectedEOFException final : public SeamException<Args...> {
	public:
		UnexpectedEOFException(const SourcePosition source_position, std::wstring exception_message, Args... args)
			: SeamException<Args...>(source_position, std::move(exception_message), std::forward<Args>(args)...) {}
	};
}
