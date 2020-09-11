#pragma once

#include <memory>
#include <string>
#include <variant>

#include "node.hpp"
#include "statement.hpp"
#include "../../lexer/lexeme.hpp"

namespace seam::core::ir::ast::expression
{
	struct expression : statement::statement
	{
		explicit expression(const utils::position_range range) :
			statement(range)
		{}

		virtual ~expression() = default;
	};
	using expression_list = std::vector<std::unique_ptr<expression>>;

	struct unary final : expression
	{
		std::unique_ptr<expression> right;
		lexer::lexeme_type operation;

		void visit(visitor* vst) override;

		explicit unary(const utils::position_range range,
			std::unique_ptr<expression> rhs,
			const lexer::lexeme_type operation) :
			expression(range),
			right(std::move(rhs)),
			operation(operation)
		{}
	};

	struct binary final : expression
	{
		std::unique_ptr<expression> left;
		std::unique_ptr<expression> right;
		lexer::lexeme_type operation;

		void visit(visitor* vst) override;

		explicit binary(const utils::position_range range,
			std::unique_ptr<expression> lhs,
			std::unique_ptr<expression> rhs,
			const lexer::lexeme_type operation) :
			expression(range),
			left(std::move(lhs)),
			right(std::move(rhs)),
			operation(operation)
		{}
	};

	struct literal : expression
	{
		void visit(visitor* vst) override = 0;

		explicit literal(const utils::position_range range) :
			expression(range)
		{}
	};

	struct bool_literal final : literal
	{
		bool value;

		void visit(visitor* vst) override;

		explicit bool_literal(const utils::position_range range, const bool value) :
			literal(range), value(value)
		{}
	};

	struct string_literal final : literal
	{
		std::string value;

		void visit(visitor* vst) override;

		explicit string_literal(const utils::position_range range, std::string value) :
			literal(range), value(std::move(value))
		{}
	};

	struct number_literal final : literal
	{
		std::variant<std::uint64_t, double> value;
		bool is_unsigned;

		void visit(visitor* vst) override;

		explicit number_literal(const utils::position_range range, const std::string& value) :
			literal(range)
		{
			if (value.find('.') != std::string::npos)
			{
				this->value = std::stod(value);
			}
			else
			{
				this->value = std::stoull(value);
				this->is_unsigned = value[0] != '-';
			}
		}
	};

	struct call final : expression 
	{
		std::string function_name;
		expression_list arguments;

		void visit(visitor* vst) override;

		explicit call(const utils::position_range range, std::string function_name, expression_list arguments) :
			expression(range), function_name(std::move(function_name)), arguments(std::move(arguments))
		{}
	};

	struct variable final : expression
	{
		std::string name;

		void visit(visitor* vst) override;

		explicit variable(const utils::position_range range, std::string name) :
			expression(range), name(std::move(name))
		{}
	};
}
