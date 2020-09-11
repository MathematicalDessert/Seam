#include <sstream>
#include <unordered_map>

#include "parser.hpp"
#include "../utils/exception.hpp"

namespace seam::core::parser
{
	struct priority
	{
		std::size_t left;
		std::size_t right;

		// if right > left then right associative
		priority(const std::size_t left, const std::size_t right) :
			left(left), right(right) {}

		priority(const std::size_t priority) :
			left(priority), right(priority) {}
	};

	const std::unordered_map<lexer::lexeme_type, priority> binary_priority
	{
		{ lexer::lexeme_type::symbol_add, 6 }, { lexer::lexeme_type::symbol_subtract, 6 },
		{ lexer::lexeme_type::symbol_multiply, 7 }, { lexer::lexeme_type::symbol_divide, 7 }, { lexer::lexeme_type::symbol_mod, 7 },
		// comparison operators
		{ lexer::lexeme_type::symbol_eq, 3 }, { lexer::lexeme_type::symbol_neq, 3 },
		{ lexer::lexeme_type::symbol_lt, 3 }, { lexer::lexeme_type::symbol_lt_eq, 3 },
		{ lexer::lexeme_type::symbol_gt, 3 }, { lexer::lexeme_type::symbol_gt_eq, 3 },
		// logical operators
		{ lexer::lexeme_type::symbol_and, 2 }, { lexer::lexeme_type::symbol_or, 1 },
	};

	// higher than any binary priority
	constexpr std::size_t unary_priority = 8;
	
	utils::position_range generate_range(const lexer::lexeme& start, const lexer::lexeme& end)
	{
		return { start.position, end.position };
	}
	
	lexer::lexeme& parser::expect(const lexer::lexeme_type type, const bool consume)
	{
		auto& current_lexeme = consume ? lexer_.next_lexeme() : lexer_.peek_lexeme();

		if (current_lexeme.type != type)
		{
			std::stringstream error_message;
			error_message << "expected " << lexer::lexeme::to_string(type) << ", got " << current_lexeme.to_string();
			throw utils::parser_exception{ current_lexeme.position, error_message.str() };
		}

		return current_lexeme;
	}

	std::shared_ptr<ir::ast::variable> parser::get_variable_from_current_block(const std::string& variable_name) const
	{
		auto* current_block = current_block_;
		while (current_block)
		{
			if (auto iterator = current_block->variables.find(variable_name); iterator != current_block->variables.cend())
			{
				return iterator->second;
			}
			current_block = current_block->parent;
		}
		
		return nullptr;
	}

	
	std::shared_ptr<types::type> parser::parse_type()
	{
		const auto type_name = std::string{ expect(lexer::lexeme_type::identifier, true).value };
		auto is_nullable = false;
		if (lexer_.peek_lexeme().type == lexer::lexeme_type::symbol_question_mark)
		{
			is_nullable = true;
			lexer_.next_lexeme();
		}
		return std::make_shared<types::type>(type_name, is_nullable, false);
	}

	std::unique_ptr<ir::ast::parameter> parser::parse_parameter()
	{
		const auto param_name = std::string{ expect(lexer::lexeme_type::identifier, true).value };
		expect(lexer::lexeme_type::symbol_colon, true);
		return std::make_unique<ir::ast::parameter>(
			param_name,
			parse_type());
	}

	ir::ast::statement::parameter_list parser::parse_parameter_list()
	{
		ir::ast::statement::parameter_list params;
		
		if (lexer_.peek_lexeme().type == lexer::lexeme_type::symbol_close_parenthesis)
		{
			return params;
		}

		params.push_back(parse_parameter());
		
		while (lexer_.peek_lexeme().type == lexer::lexeme_type::symbol_comma)
		{
			lexer_.next_lexeme();
			params.push_back(parse_parameter());
		}

		return params;
	}

	ir::ast::expression::expression_list parser::parse_expression_list()
	{
		ir::ast::expression::expression_list list;
		if (lexer_.peek_lexeme().type == lexer::lexeme_type::symbol_close_parenthesis)
		{
			return list;
		}

		list.emplace_back(parse_expression().first);

		while (lexer_.peek_lexeme().type == lexer::lexeme_type::symbol_comma)
		{
			lexer_.next_lexeme();
			list.emplace_back(parse_expression().first);
		}
		
		return list;
	}

	std::unique_ptr<ir::ast::expression::call> parser::parse_call_expression(std::string function_name)
	{
		const auto current_lexeme = expect(lexer::lexeme_type::symbol_open_parenthesis, true);

		auto arguments = ir::ast::expression::expression_list{};
		if (lexer_.peek_lexeme().type != lexer::lexeme_type::symbol_close_parenthesis)
		{
			arguments = parse_expression_list();;
		}
		
		expect(lexer::lexeme_type::symbol_close_parenthesis, true);

		return std::make_unique<ir::ast::expression::call>(
			generate_range(current_lexeme, lexer_.peek_lexeme()),
			std::move(function_name),
			std::move(arguments)
			);
	}

	std::unique_ptr<ir::ast::expression::expression> parser::parse_simple_expression(const bool use_current_lexeme)
	{
		const auto current_lexeme = use_current_lexeme ? lexer_.current_lexeme() : lexer_.next_lexeme(); // DON'T USE REFERENCE

		switch (current_lexeme.type)
		{
			case lexer::lexeme_type::symbol_open_parenthesis:
			{
				auto expression = parse_expression();
				expect(lexer::lexeme_type::symbol_close_parenthesis, true);
				return std::move(expression.first);
			}
			case lexer::lexeme_type::identifier: // parse call expression
			{
				const auto is_function_call = lexer_.peek_lexeme().type == lexer::lexeme_type::symbol_open_parenthesis;
				auto name = std::string{ current_lexeme.value };
					
				if (is_function_call) // must be call expression
				{
					return parse_call_expression(std::move(name));
				}

				if (!get_variable_from_current_block(name))
				{
					std::stringstream error_message;
					error_message << "use of undeclared variable '" << name << "'";
					throw utils::parser_exception{ lexer_.peek_lexeme().position, error_message.str() };
				}
					
				return std::make_unique<ir::ast::expression::variable>(
					generate_range(current_lexeme, lexer_.peek_lexeme()),
					std::move(name)
					);
			}
			case lexer::lexeme_type::keyword_true:
			case lexer::lexeme_type::keyword_false:
			{
				return std::make_unique<ir::ast::expression::bool_literal>(
					generate_range(current_lexeme, lexer_.peek_lexeme()),
					current_lexeme.type == lexer::lexeme_type::keyword_true);
			}
			case lexer::lexeme_type::string_literal:
			{
				return std::make_unique<ir::ast::expression::string_literal>(
					generate_range(current_lexeme, lexer_.peek_lexeme()),
					std::string{ current_lexeme.value });
			}
			case lexer::lexeme_type::number_literal:
			{
				return std::make_unique<ir::ast::expression::number_literal>(
					generate_range(current_lexeme, lexer_.peek_lexeme()),
					std::string{ current_lexeme.value });
			}
			default:
			{
				std::stringstream error_message;
				error_message << "expected expression, got " << current_lexeme.to_string();
				throw utils::parser_exception{ current_lexeme.position, error_message.str() };
			}
		}
	}

	std::pair<std::unique_ptr<ir::ast::expression::expression>, std::optional<lexer::lexeme_type>> parser::parse_expression(const std::size_t right_binding_power, bool use_current_lexeme)
	{
		const auto& first_lexeme = lexer_.peek_lexeme();

		std::unique_ptr<ir::ast::expression::expression> expression;
		if (is_unary_operator(first_lexeme.type))
		{
			const auto operator_type = lexer_.next_lexeme().type;

			expression = std::make_unique<ir::ast::expression::unary>(
				generate_range(first_lexeme, lexer_.peek_lexeme()),
				parse_expression(unary_priority).first,
				operator_type);
		}
		else
		{
			expression = parse_simple_expression(use_current_lexeme);
		}

		auto operator_type = lexer_.peek_lexeme().type;
		while (true)
		{
			auto it = binary_priority.find(operator_type);
			if (it == binary_priority.cend() || right_binding_power >= it->second.left)
			{
				break;
			}

			lexer_.next_lexeme();

			auto [next_expression, next_operator_type] = parse_expression(it->second.right);

			expression = std::make_unique<ir::ast::expression::binary>(
				generate_range(first_lexeme, lexer_.peek_lexeme()),
				std::move(expression), 
				std::move(next_expression),
				operator_type);

			if (!next_operator_type)
			{
				break;
			}

			operator_type = next_operator_type.value();
		}
		
		return { std::move(expression), operator_type };
	}

	std::unique_ptr<ir::ast::statement::ret_stat> parser::parse_return_statement()
	{
		const auto return_keyword = lexer_.next_lexeme();

		auto expression = parse_expression();
		
		return std::make_unique<ir::ast::statement::ret_stat>(
			generate_range(return_keyword, lexer_.peek_lexeme()),
			std::move(expression.first));
	}

	std::unique_ptr<ir::ast::statement::statement> parser::parse_assignment_statement(std::string variable_name)
	{
		const auto symbol_lexeme = lexer_.next_lexeme();

		switch (symbol_lexeme.type)
		{
			case lexer::lexeme_type::symbol_assign: // <> = <>
			{
				if (!get_variable_from_current_block(variable_name))
				{
					std::stringstream error_message;
					error_message << "cannot assign undeclared variable '" << variable_name << "', did you mean to declare?";
					throw utils::parser_exception{ lexer_.peek_lexeme().position, error_message.str() };
				}
				return std::make_unique<ir::ast::statement::variable_assignment>(
					generate_range(symbol_lexeme, lexer_.peek_lexeme()),
					std::move(variable_name),
					parse_expression().first);
			}
			case lexer::lexeme_type::symbol_colon_assign: // <> := <>
			case lexer::lexeme_type::symbol_colon: // <>: <> = <>
			{
				if (get_variable_from_current_block(variable_name))
				{
					std::stringstream error_message;
					error_message << "cannot re-declare variable '" << variable_name << "', did you mean to assign?";
					throw utils::parser_exception{ symbol_lexeme.position, error_message.str() };
				}

				std::shared_ptr<types::type> var_type = nullptr;
				if (symbol_lexeme.type == lexer::lexeme_type::symbol_colon_assign)
				{
					var_type = std::make_shared<types::type>("", false, true);
				}
				else
				{
					var_type = parse_type();
					expect(lexer::lexeme_type::symbol_assign, true);
				}				

				current_block_->variables.emplace(variable_name, std::make_shared<ir::ast::variable>(variable_name, var_type));
					
				return std::make_unique<ir::ast::statement::variable_assignment>(
					generate_range(symbol_lexeme, lexer_.peek_lexeme()),
					std::move(variable_name),
					parse_expression().first);
			}
			default:
			{
				std::stringstream error_message;
				error_message << "expected assignment :=, =, or :, got " << symbol_lexeme.to_string();
				throw utils::parser_exception{ lexer_.peek_lexeme().position, error_message.str() };
			}
		}
	}
	
	std::unique_ptr<ir::ast::statement::statement_block> parser::parse_statement_block(const ir::ast::statement::parameter_list* vars)
	{
		const auto& open_block = expect(lexer::lexeme_type::symbol_open_brace, true);
		auto* const old_block = current_block_;
		auto new_block = std::make_unique<ir::ast::statement::statement_block>(generate_range(open_block, lexer_.peek_lexeme()));
		new_block->parent = current_block_;
		current_block_ = new_block.get();

		if (vars)
		{
			for (const auto& param : *vars)
			{
				current_block_->variables.emplace(
					param->name,
					std::make_shared<ir::ast::variable>(
						param->name,
						param->type)
				);
			}
		}
		
		ir::ast::statement::statement_list body;
		
		while (true)
		{
			const auto peeked_lexeme = lexer_.peek_lexeme();

			if (peeked_lexeme.type == lexer::lexeme_type::symbol_close_brace)
			{
				break;
			}
			
			switch (peeked_lexeme.type)
			{
				case lexer::lexeme_type::symbol_open_brace: // general block def
				{
					body.emplace_back(parse_statement_block());
					break;
				}
				case lexer::lexeme_type::keyword_return: // return statement
				{
					body.emplace_back(parse_return_statement());
					break;
				}
				case lexer::lexeme_type::identifier: // assignment
				{
					const auto var_name = std::string{ lexer_.next_lexeme().value };
						
					const auto identifier_peek = lexer_.peek_lexeme();
					if (identifier_peek.type == lexer::lexeme_type::symbol_assign
						|| identifier_peek.type == lexer::lexeme_type::symbol_colon
						|| identifier_peek.type == lexer::lexeme_type::symbol_colon_assign)
					{
						body.emplace_back(parse_assignment_statement(var_name));
						break;
					}
					[[fallthrough]];
				}
				default:
				{
					auto expression = parse_expression(0, peeked_lexeme.type == lexer::lexeme_type::identifier);

					body.emplace_back(std::move(expression.first));
					break;
				}
			}
			
		}

		const auto& close_block = expect(lexer::lexeme_type::symbol_close_brace, true);

		current_block_ = old_block;
		new_block->range = generate_range(open_block, close_block),
		new_block->body = std::move(body);
		return std::move(new_block);
	}
	
	std::unique_ptr<ir::ast::statement::function_definition> parser::parse_function_definition()
	{
		const auto& kw_lexeme = lexer_.next_lexeme(); // consume function keyword

		expect(lexer::lexeme_type::identifier);
		auto function_name = std::string{ lexer_.next_lexeme().value };

		expect(lexer::lexeme_type::symbol_open_parenthesis, true);
		auto parameter_list = parse_parameter_list();
		expect(lexer::lexeme_type::symbol_close_parenthesis, true);

		auto return_type = std::make_shared<types::type>("", false, true);
		if (lexer_.peek_lexeme().type == lexer::lexeme_type::symbol_arrow)
		{
			lexer_.next_lexeme();
			return_type = parse_type();
		}

		std::vector<std::string> attributes;
		while (lexer_.peek_lexeme().type == lexer::lexeme_type::attribute)
		{
			attributes.emplace_back(std::string{ lexer_.next_lexeme().value });
		}
		
		auto function_body = parse_statement_block(&parameter_list);

		return std::make_unique<ir::ast::statement::function_definition>(
			generate_range(kw_lexeme, lexer_.peek_lexeme()),
			std::move(function_name),
			std::move(return_type),
			std::move(parameter_list),
			std::move(attributes),
			std::move(function_body));
	}

	std::unique_ptr<ir::ast::statement::declaration> parser::parse_declaration()
	{
		const auto& lexeme = lexer_.peek_lexeme();

		switch (lexeme.type)
		{
			case lexer::lexeme_type::keyword_fn:
			{
				return parse_function_definition();
			}
			default:
			{
				throw utils::parser_exception{ lexeme.position, "unexpected lexeme in restricted block" };
			}
		}
	}

	std::unique_ptr<ir::ast::statement::declaration_block> parser::parse_declaration_block(const bool is_type_scope)
	{
		ir::ast::statement::declaration_list body;

		const auto& start = lexer_.peek_lexeme();
		auto* const old_block = current_block_;
		auto new_block = std::make_unique<ir::ast::statement::declaration_block>(generate_range(start, lexer_.peek_lexeme()));
		new_block->parent = current_block_;
		current_block_ = new_block.get();
		
		while (true)
		{
			const auto& lexeme = lexer_.peek_lexeme();

			if (!is_type_scope && lexeme.type == lexer::lexeme_type::eof
				|| is_type_scope && lexeme.type == lexer::lexeme_type::symbol_close_brace)
			{
				break;
			}

			body.emplace_back(parse_declaration());
		}

		current_block_ = old_block;
		new_block->range = generate_range(start, lexer_.peek_lexeme());
		new_block->body = std::move(body);
		return std::move(new_block);
	}

	parser::parser(const std::string_view source) :
		lexer_({ source })
	{}

	std::unique_ptr<ir::ast::statement::declaration_block> parser::parse()
	{
		auto root = parse_declaration_block();

		expect(lexer::lexeme_type::eof);

		return std::move(root);
	}
}
