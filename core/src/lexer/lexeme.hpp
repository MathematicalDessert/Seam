#pragma once

#include <string_view>

#include "../utils/position.hpp"

namespace seam::core::lexer
{
	enum class lexeme_type
	{
		eof,
		identifier,
		attribute,
		
		string_literal,
		number_literal,
		
		symbol_separator,
		symbol_comma,
		symbol_colon,
		symbol_question_mark,
		symbol_arrow,
		symbol_open_brace,
		symbol_close_brace,
		symbol_open_parenthesis,
		symbol_close_parenthesis,
		symbol_assign,
		symbol_colon_assign,
		symbol_add,
		symbol_add_assign,
		symbol_subtract,
		symbol_subtract_assign,
		symbol_multiply,
		symbol_multiply_assign,
		symbol_divide,
		symbol_divide_assign,
		symbol_mod,

		symbol_eq,
		symbol_neq,
		symbol_lt,
		symbol_lt_eq,
		symbol_gt,
		symbol_gt_eq,
		
		symbol_and,
		symbol_or,
		symbol_not,
		
		keyword_fn,
		keyword_type,
		keyword_while,
		keyword_if,
		keyword_elseif,
		keyword_else,
		keyword_import,
		keyword_true,
		keyword_false,
		keyword_return,
		
		count, // this is quite ugly, but there seems to be no better way.
	};

	constexpr static std::string_view lexeme_type_name_array[] = {
		"<eof>",
		"<identifier>",
		"<attribute>",
		"<string literal>",
		"<number literal>",
		".",
		",",
		":",
		"?",
		"->",
		"{",
		"}",
		"(",
		")",
		"=",
		":=",
		"+",
		"+=",
		"-",
		"-=",
		"*",
		"*=",
		"/",
		"/=",
		"%",
		"==",
		"!=",
		"<",
		"<=",
		">",
		">=",
		"&&",
		"||",
		"!",
		"fn",
		"type",
		"while",
		"if",
		"elseif",
		"else",
		"import",
		"true",
		"false",
		"return",
		"<UNUSED_COUNT>"
	};
	static_assert(static_cast<int>(lexeme_type::count) + 1 == std::size(lexeme_type_name_array), "lexeme_type and lexeme_type_name_array size mismatch");
	
	static bool is_numerical_operator(const lexeme_type type)
	{
		switch (type)
		{
			case lexeme_type::symbol_divide:
			case lexeme_type::symbol_divide_assign:
			case lexeme_type::symbol_add:
			case lexeme_type::symbol_add_assign:
			case lexeme_type::symbol_subtract:
			case lexeme_type::symbol_subtract_assign:
			case lexeme_type::symbol_multiply:
			case lexeme_type::symbol_multiply_assign:
			case lexeme_type::symbol_mod:
			{
				return true;
			}
			default:
			{
				return false;
			}
		}
	}

	static bool is_comparison_operator(const lexeme_type type)
	{
		switch (type)
		{
			case lexeme_type::symbol_eq:
			case lexeme_type::symbol_neq:
			case lexeme_type::symbol_lt:
			case lexeme_type::symbol_lt_eq:
			case lexeme_type::symbol_gt:
			case lexeme_type::symbol_gt_eq:
			case lexeme_type::symbol_not:
			{
				return true;
			}
			default:
			{
				return false;
			}
		}
	}

	static bool is_operator(const lexeme_type type)
	{
		return is_comparison_operator(type) || is_numerical_operator(type);
	}

	static bool is_unary_operator(const lexeme_type type)
	{
		switch (type)
		{
			case lexeme_type::symbol_subtract:
			case lexeme_type::symbol_not:
			{
				return true;
			}
			default:
			{
				return false;
			}
		}
	}

	static bool is_binary_operator(const lexeme_type type)
	{
		return is_operator(type) && !is_unary_operator(type);
	}
	
	struct lexeme
	{
		lexeme_type type = lexeme_type::eof;
		std::string_view value{};
		utils::position position{ 0, 0 };
		
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
