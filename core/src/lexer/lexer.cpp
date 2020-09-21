#include <cctype>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

#include "lexer.hpp"
#include "../module.hpp"
#include "../utils/exception.hpp"

namespace seam::core::lexer
{
	using lexeme_map_t = std::unordered_map<std::string_view, lexeme_type>;
	const lexeme_map_t keyword_map
	{
		{ "fn", lexeme_type::keyword_fn },
		{ "type", lexeme_type::keyword_type },
		{ "while", lexeme_type::keyword_while },
		{ "if", lexeme_type::keyword_if },
		{ "elseif", lexeme_type::keyword_elseif },
		{ "else", lexeme_type::keyword_else },
		{ "import", lexeme_type::keyword_import },
		{ "true", lexeme_type::keyword_true },
		{ "false", lexeme_type::keyword_false },
		{ "return", lexeme_type::keyword_return },
	};

	const lexeme_map_t symbol_map
	{
		{ ".", lexeme_type::symbol_separator },
		{ ",", lexeme_type::symbol_comma },
		{ ":", lexeme_type::symbol_colon },
		{ "?", lexeme_type::symbol_question_mark },
		{ "->", lexeme_type::symbol_arrow },
		{ "{", lexeme_type::symbol_open_brace },
		{ "}", lexeme_type::symbol_close_brace },
		{ "(", lexeme_type::symbol_open_parenthesis },
		{ ")", lexeme_type::symbol_close_parenthesis },
		{ "=", lexeme_type::symbol_assign },
		{ ":=", lexeme_type::symbol_colon_assign },
		{ "+", lexeme_type::symbol_add },
		{ "+=", lexeme_type::symbol_add_assign },
		{ "-", lexeme_type::symbol_subtract },
		{ "-=", lexeme_type::symbol_subtract },
		{ "*", lexeme_type::symbol_multiply },
		{ "*=", lexeme_type::symbol_multiply_assign },
		{ "%", lexeme_type::symbol_mod },
		{ "==", lexeme_type::symbol_eq },
		{ "!=", lexeme_type::symbol_neq },
		{ "<", lexeme_type::symbol_lt },
		{ "<=", lexeme_type::symbol_lt_eq },
		{ ">", lexeme_type::symbol_gt },
		{ ">=", lexeme_type::symbol_gt_eq },
		{ "&&", lexeme_type::symbol_and },
		{ "||", lexeme_type::symbol_or },
		{ "!", lexeme_type::symbol_not },
	};

	const std::unordered_set<std::string> attributes = { "constructor", "export" };
	
	bool is_start_identifier_char(const char value)
	{
		return std::isalpha(value) || value == '_';
	}

	bool is_identifier_char(const char value)
	{
		return std::isalnum(value) || value == '_';
	}

	bool is_keyword_char(const char value)
	{
		return value != '_' && !std::isupper(value);
	}
	
	utils::position lexer::current_position() const
	{
		return { line_, read_offset_ - line_start_offset_ };
	}

	char lexer::peek_character(const std::size_t offset) const
	{
		return read_offset_ + offset >= module_->source_.length() ? eof_character : module_->source_[read_offset_ + offset];
	}

	void lexer::consume_character()
	{
		if (peek_character() == '\n')
		{
			++line_;
			line_start_offset_ = read_offset_;
		}
		++read_offset_;
	}

	void lexer::skip_whitespace()
	{
		auto next_character = peek_character();
		while (next_character != eof_character && std::isspace(next_character))
		{
			consume_character();
			next_character = peek_character();
		}
	}

	void lexer::skip_comment()
	{
		consume_character();
		const auto is_long_comment = peek_character() == '/';

		while (true)
		{
			const auto next_character = peek_character();
			if (!is_long_comment && next_character == '\n')
			{
				consume_character();
				return;
			}

			if (is_long_comment && next_character == '/' && peek_character(1) == '/' && peek_character(2) == '/')
			{
				consume_character();
				consume_character();
				consume_character();
				return;
			}

			if (is_long_comment && next_character == eof_character)
			{
				throw utils::lexical_exception{ current_position(), "unterminated long comment" };
			}

			consume_character();
		}
	}

	void lexer::lex_string_literal(lexeme& ref)
	{
		consume_character();
		const auto start_offset = read_offset_;

		while (true)
		{
			if (peek_character() == '\\' && peek_character(1) == '"') // String escape
			{
				consume_character();
				consume_character();
				continue;
			}

			if (peek_character() == '"')
			{
				ref.type = lexeme_type::string_literal;
				ref.value = module_->source_.substr(start_offset, read_offset_ - start_offset);
				consume_character();
				return;
			}

			if (peek_character() == eof_character)
			{
				throw utils::lexical_exception{ current_position(), "unclosed string literal" };
			}

			consume_character();
		}
	}

	void lexer::lex_number_literal(lexeme& ref)
	{
		const auto is_hex = peek_character() == '0' && peek_character(1) == 'x';
		auto is_float = false;

		const auto start_offset = read_offset_;

		if (is_hex)
		{
			consume_character();
			consume_character(); // Consume 0x.
		}

		while (true)
		{
			if (peek_character() == '.')
			{
				is_float = true;
				consume_character();
			}

			if (is_hex && is_float)
			{
				throw utils::lexical_exception{
					current_position(),
					"malformed number"
				};
			}

			const auto next_char = peek_character(); // Not yet parsed correctly I think?
			if ((is_hex ? std::isxdigit(next_char) : std::isdigit(next_char)) || next_char == '_')
			{
				consume_character();
			}
			else
			{
				break;
			}
		}

		ref.type = lexeme_type::number_literal;
		ref.value = module_->source_.substr(start_offset, read_offset_ - start_offset);
	}

	void lexer::lex_keyword_or_identifier(lexeme& ref)
	{
		auto can_be_keyword = is_keyword_char(peek_character());

		const auto start_offset = read_offset_;
		consume_character();

		while (true)
		{
			const auto next_char = peek_character();

			if (!is_keyword_char(next_char))
			{
				can_be_keyword = false;
			}

			if (!is_identifier_char(next_char))
			{
				ref.type = lexeme_type::identifier;
				ref.value = module_->source_.substr(start_offset, read_offset_ - start_offset);
				break;
			}

			consume_character();
		}

		if (can_be_keyword)
		{
			if (const auto keyword_type = keyword_map.find(module_->source_.substr(start_offset, read_offset_ - start_offset)); keyword_type != keyword_map.cend())
			{
				ref.type = keyword_type->second;
			}
		}
	}

	void lexer::lex_attribute(lexeme& ref)
	{
		consume_character();

		const auto start_offset = read_offset_;

		if (!is_start_identifier_char(peek_character()))
		{
			throw utils::lexical_exception{
				current_position(),
				"unexpected symbol"
			};
		}

		consume_character();

		while (true)
		{
			const auto next_char = peek_character();

			if (!is_identifier_char(next_char))
			{
				const auto proposed_attribute = module_->source_.substr(start_offset, read_offset_ - start_offset);
				if (attributes.find(std::string{ proposed_attribute }) == attributes.cend())
				{
					std::stringstream error_message;
					error_message << "unknown attribute: '" << proposed_attribute << "'";
					throw utils::lexical_exception{ current_position(), error_message.str() };
				}

				ref.type = lexeme_type::attribute;
				ref.value = proposed_attribute;
				return;
			}

			consume_character();
		}
	}

	void lexer::lex_symbol(lexeme& ref)
	{
		const auto start_offset = read_offset_;

		consume_character();
		lexeme_map_t::const_iterator symbol;
		lexeme_type type;
		if (peek_character(1) != eof_character && (symbol = symbol_map.find(module_->source_.substr(start_offset, 2))) != symbol_map.cend())
		{
			consume_character();
			type = symbol->second;
		}
		else if (symbol = symbol_map.find(module_->source_.substr(start_offset, 1)); symbol != symbol_map.cend())
		{
			type = symbol->second;
		}
		else
		{
			throw utils::lexical_exception{
				current_position(),
				"unexpected symbol"
			};
		}

		ref.type = type;
	}
	
	void lexer::lex(lexeme& ref)
	{
		skip_whitespace();

		ref.position = current_position();
		
		switch (peek_character())
		{
			case eof_character:
			{
				ref.type = lexeme_type::eof;
				break;
			}
			case '/':
			{
				consume_character();
				const auto next_character = peek_character();

				if (next_character == '/')
				{
					skip_comment();
					return lex(ref);
				}

				if (next_character == '=')
				{
					consume_character();
					ref.type = lexeme_type::symbol_divide_assign;
					break;
				}
					
				ref.type = lexeme_type::symbol_divide;
				break;
			}
			case '"':
			{
				return lex_string_literal(ref);
			}
			case '@':
			{
				return lex_attribute(ref);
			}
			default:
			{
				if (is_start_identifier_char(peek_character()))
				{
					return lex_keyword_or_identifier(ref);
				}

				if (std::isdigit(peek_character()))
				{
					return lex_number_literal(ref);
				}

				return lex_symbol(ref);
			}
		}
	}
	
	lexer::lexer(module* module) :
		module_(module)
	{}
	
	lexeme& lexer::peek_lexeme()
	{
		if (!peeked_lexeme_)
		{
			peeked_lexeme_ = lexeme{};
			lex(*peeked_lexeme_);
		}
		return peeked_lexeme_.value();
	}

	lexeme& lexer::current_lexeme()
	{
		return *current_lexeme_;
	}
	
	lexeme& lexer::next_lexeme()
	{
		if (peeked_lexeme_)
		{
			current_lexeme_ = *peeked_lexeme_;
			peeked_lexeme_.reset();
		}
		else
		{
			lex(*current_lexeme_);
		}
		return *current_lexeme_;
	}
}
