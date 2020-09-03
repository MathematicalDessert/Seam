#pragma once

#include <string_view>

namespace seam::core::lexer
{
	enum class lexeme_type
	{
		eof,
		identifier,
		attribute,
		
		string_literal,
		number_literal,

		op_divide,
		op_divide_assign,
		
		symbol_separator,
		symbol_open_parenthesis,
		symbol_close_parenthesis,

		keyword_fn,
		keyword_import,
		
		count, // this is quite ugly, but there seems to be no better way.
	};

	constexpr static std::string_view lexeme_type_name_array[] = {
		"<eof>",
		"<identifier>",
		"<attribute>",
		"<string literal>",
		"<number literal>",
		"/",
		"/=",
		".",
		"(",
		")",
		"fn",
		"import",
		"<UNUSED_COUNT>"
	};
	static_assert(static_cast<int>(lexeme_type::count) + 1 == std::size(lexeme_type_name_array), "lexeme_type and lexeme_type_name_array size mismatch");
	
	struct lexeme
	{
		lexeme_type type = lexeme_type::eof;
		std::string_view value{};

		static std::string to_string(const lexeme_type type)
		{
			return std::string{ lexeme_type_name_array[static_cast<int>(type)] };
		}

		[[nodiscard]] std::string to_string() const
		{
			switch (type)
			{
				case lexeme_type::number_literal:
				case lexeme_type::identifier:
				{
					return { value.cbegin(), value.cend() };
				}
				case lexeme_type::attribute:
				{
					return '@' + std::string{ value.cbegin(), value.cend() };
				}
				case lexeme_type::string_literal:
				{
					return '"' + std::string{ value.cbegin(), value.cend() } + '"';
				}
				default:
				{
					return to_string(type);
				}
			}
		}

		explicit lexeme() = default;
		
		explicit lexeme(const lexeme_type type) :
			type(type)
		{}
		
		explicit lexeme(const lexeme_type type, const std::string_view value) :
			type(type), value(value)
		{}
	};
}
