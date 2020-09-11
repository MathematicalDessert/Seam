#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <vector>

#include "node.hpp"
#include "../../types/type.hpp"

namespace seam::core::ir::ast
{
	namespace expression
	{
		struct expression;
	}

	struct parameter
	{
		std::string name;
		std::shared_ptr<types::type> type;

		explicit parameter(std::string name, std::shared_ptr<types::type> type) :
			name(std::move(name)), type(std::move(type))
		{}
	};

	struct variable
	{
		std::string name;
		std::shared_ptr<types::type> type;

		explicit variable(std::string name, std::shared_ptr<types::type> type) :
			name(std::move(name)), type(std::move(type))
		{}
	};
	
	namespace statement
	{
		struct statement : node
		{
			explicit statement(const utils::position_range range) :
				node(range)
			{}

			virtual ~statement() = default;
		};
		using statement_list = std::vector<std::unique_ptr<statement>>;

		struct block : statement
		{
			block* parent = nullptr;
			std::unordered_map<std::string, std::shared_ptr<variable>> variables;

			explicit block(const utils::position_range range) :
				statement(range)
			{}
		};

		struct statement_block final : block
		{
			statement_list body;

			void visit(visitor* vst) override;

			explicit statement_block(const utils::position_range range) :
				block(range)
			{}
			
			explicit statement_block(const utils::position_range range, statement_list body) :
				block(range), body(std::move(body))
			{}
		};

		struct declaration : statement
		{
			using statement::statement;
		};
		using declaration_list = std::vector<std::unique_ptr<declaration>>;

		struct declaration_block final : block
		{
			declaration_list body;

			void visit(visitor* vst) override;

			explicit declaration_block(const utils::position_range range) :
				block(range)
			{}

			explicit declaration_block(const utils::position_range range, declaration_list body) :
				block(range), body(std::move(body))
			{}
		};

		using parameter_list = std::vector<std::unique_ptr<parameter>>;

		struct function_declaration : declaration
		{
			std::string name;
			std::shared_ptr<types::type> return_type;
			parameter_list parameters;
			std::vector<std::string> attributes;

			explicit function_declaration(const utils::position_range range) :
				declaration(range)
			{}
			
			explicit function_declaration(const utils::position_range range, std::string function_name, std::shared_ptr<types::type> return_type, parameter_list parameters, std::vector<std::string> attributes) :
				declaration(range), name(std::move(function_name)), return_type(std::move(return_type)), parameters(std::move(parameters)), attributes(std::move(attributes))
			{}
		};

		struct function_definition final : function_declaration
		{
			std::unique_ptr<statement_block> block;

			void visit(visitor* vst) override;

			explicit function_definition(const utils::position_range range) :
				function_declaration(range)
			{}
			
			explicit function_definition(const utils::position_range range, std::string function_name, std::shared_ptr<types::type> return_type, parameter_list parameters, std::vector<std::string> attributes, std::unique_ptr<statement_block> block) :
				function_declaration(range, std::move(function_name), std::move(return_type), std::move(parameters), std::move(attributes)), block(std::move(block))
			{}
		};

		struct variable_declaration final : statement
		{
			std::string variable_name;
			std::unique_ptr<expression::expression> value;

			void visit(visitor* vst) override;

			explicit variable_declaration(const utils::position_range range, std::string variable_name, std::unique_ptr<expression::expression> value) :
				statement(range), variable_name(std::move(variable_name)), value(std::move(value))
			{}
		};

		struct variable_assignment final : statement
		{
			std::string variable_name;
			std::unique_ptr<expression::expression> value;

			void visit(visitor* vst) override;

			explicit variable_assignment(const utils::position_range range, std::string variable_name, std::unique_ptr<expression::expression> value) :
				statement(range), variable_name(std::move(variable_name)), value(std::move(value))
			{}
		};

		struct ret_stat final : statement
		{
			std::unique_ptr<expression::expression> value;

			void visit(visitor* vst) override;

			explicit ret_stat(const utils::position_range range, std::unique_ptr<expression::expression> value) :
				statement(range), value(std::move(value))
			{}
		};
	}
}