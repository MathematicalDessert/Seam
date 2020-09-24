#pragma once

#include <unordered_map>
#include <memory>
#include <set>
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
			std::string name;
			block* parent = nullptr;
			std::shared_ptr<types::type> return_type;

			std::unordered_map<std::string, std::shared_ptr<variable>> variables;

			std::shared_ptr<ir::ast::variable> get_variable(const std::string& variable_name);

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
			std::string mangled_name;
			std::shared_ptr<types::type> return_type;
			parameter_list parameters;
			std::set<std::string> attributes;

			explicit function_declaration(const utils::position_range range) :
				declaration(range)
			{}
			
			explicit function_declaration(const utils::position_range range, std::string module_name, std::string function_name, std::shared_ptr<types::type> return_type, parameter_list parameters, std::set<std::string> attributes) :
				declaration(range), name(std::move(function_name)), return_type(std::move(return_type)), parameters(std::move(parameters)), attributes(std::move(attributes))
			{
				mangled_name = module_name + "@" + name;
			}
		};

		struct function_definition final : function_declaration
		{
			std::unique_ptr<statement_block> block;

			void visit(visitor* vst) override;

			explicit function_definition(const utils::position_range range) :
				function_declaration(range)
			{}
			
			explicit function_definition(const utils::position_range range, std::string module_name, std::string function_name, std::shared_ptr<types::type> return_type, parameter_list parameters, std::set<std::string> attributes, std::unique_ptr<statement_block> block) :
				function_declaration(range, module_name, std::move(function_name), std::move(return_type), std::move(parameters), std::move(attributes)), block(std::move(block))
			{}
		};

		struct type_declaration : declaration
		{
			std::string name;

			explicit type_declaration(const utils::position_range range, std::string name) :
				declaration(range), name(std::move(name))
			{}
		};

		struct type_alias_declaration final: type_declaration
		{
			std::shared_ptr<types::type> aliased_type;

			void visit(visitor* vst) override;

			explicit type_alias_declaration(const utils::position_range range, std::string name, std::shared_ptr<types::type> aliased_type) :
				type_declaration(range, std::move(name)), aliased_type(aliased_type)
			{}
		};

		struct type_class_definition final : type_declaration
		{
			parameter_list fields;
			std::unique_ptr<declaration_block> body;
			
			void visit(visitor* vst) override;

			explicit type_class_definition(const utils::position_range range, std::string name, parameter_list fields, std::unique_ptr<declaration_block> body) :
				type_declaration(range, std::move(name)), fields(std::move(fields)), body(std::move(body))
			{}
		};

		struct variable_declaration final : statement // unused???
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
			std::shared_ptr<types::type> type;
			std::unique_ptr<expression::expression> value;

			void visit(visitor* vst) override;

			explicit variable_assignment(const utils::position_range range, std::string variable_name, std::shared_ptr<types::type> type, std::unique_ptr<expression::expression> value) :
				statement(range), variable_name(std::move(variable_name)), type(std::move(type)), value(std::move(value))
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

		struct while_stat final : statement
		{
			std::unique_ptr<expression::expression> condition;
			std::unique_ptr<statement_block> body;

			void visit(visitor* vst) override;

			explicit while_stat(const utils::position_range range, std::unique_ptr<expression::expression> condition, std::unique_ptr<statement_block> body) :
				statement(range), condition(std::move(condition)), body(std::move(body))
			{}
		};

		struct if_stat final : statement
		{
			std::unique_ptr<expression::expression> condition;

			std::unique_ptr<statement_block> body;
			std::unique_ptr<statement_block> else_body;

			void visit(visitor* vst) override;

			explicit if_stat(const utils::position_range range, std::unique_ptr<expression::expression> condition, std::unique_ptr<statement_block> body, std::unique_ptr<statement_block> else_body) :
				statement(range), condition(std::move(condition)), body(std::move(body)), else_body(std::move(else_body))
			{}
		};
	}
}