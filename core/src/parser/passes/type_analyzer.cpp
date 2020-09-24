#include <sstream>

#include "../../utils/exception.hpp"
#include "type_analyzer.hpp"
#include "../../ir/ast/visitor.hpp"

namespace seam::core::parser::passes
{
	struct type_analyzer_visitor : ir::ast::visitor
	{
		ir::ast::statement::block* current_block;
		types::built_in_type current_type;

		bool visit(ir::ast::expression::bool_literal* node) override
		{
			current_type = types::built_in_type::bool_;
			return false;
		}

		bool visit(ir::ast::expression::number_literal* node) override
		{
			current_type = types::get_smallest_viable_number_type(node->value, node->is_unsigned);
			
			if (current_type == types::built_in_type::undefined)
			{
				throw utils::type_exception(node->range.start, "invalid number");
			}
			return false;
		}

		bool visit(ir::ast::expression::variable* node) override
		{
			current_type = current_block->get_variable(node->name)->type->internal_type;
			return false;
		}

		bool visit(ir::ast::expression::binary* node) override
		{
			node->left->visit(this);
			auto left_type = current_type;

			node->right->visit(this);
			auto right_type = current_type;

			if (lexer::is_numerical_operator(node->operation)) // make sure both left and right types are numbers
			{
				if (!types::is_number(left_type) || !types::is_number(right_type))
				{
					std::stringstream error_message;
					error_message
						<< "operator '"
						<< lexer::lexeme::to_string(node->operation)
						<< "' must be used on number types";

					throw utils::type_exception(node->range.start, error_message.str());
				}

				// check types are the same and coerce...
			}

			if (left_type != right_type)
			{
				// TODO: check if rhs can be coerced into lhs
			}
			
			if (is_comparison_operator(node->operation))
			{
				current_type = types::built_in_type::bool_;
			}
			else
			{
				current_type = left_type;
			}
			return false;
		}

		bool visit(ir::ast::statement::variable_assignment* node) override
		{
			if (node->value) // variable has immediate value
			{
				node->value->visit(this);
				
				if (node->type->is_implicit_type) // "deduce" type
				{
					node->type->internal_type = current_type;
				}
				else if (node->type && node->type->internal_type != types::built_in_type::undefined && current_type != node->type->internal_type)
				{
					// coerce rhs into lhs type (trim + expand)
					auto coerced_type = coerce_type(node->type->internal_type, current_type);
					if (coerced_type == types::built_in_type::undefined)
					{
						std::stringstream error_message;
						error_message
							<< "type mismatch: expected "
							<< types::built_in_type_name_array[static_cast<int>(node->type->internal_type)]
							<< ", got "
							<< types::built_in_type_name_array[static_cast<int>(current_type)];

						throw utils::type_exception(node->range.start, error_message.str());
					}

					// set coerced type maybe????
				}
			}
			return false;
		}

		bool visit(ir::ast::statement::ret_stat* node) override
		{
			if (current_block->return_type->is_implicit_type && current_block->return_type->internal_type == types::built_in_type::undefined)
			{
				node->value->visit(this);
				current_block->return_type->internal_type = current_type;
			}
			else
			{
				node->value->visit(this);

				auto coerced_type = coerce_type(current_block->return_type->internal_type, current_type);
				if (coerced_type == types::built_in_type::undefined)
				{
					std::stringstream error_message;
					error_message
						<< "incompatible return types: "
						<< types::built_in_type_name_array[static_cast<int>(current_block->return_type->internal_type)]
						<< ", tried to return type "
						<< types::built_in_type_name_array[static_cast<int>(current_type)];

					throw utils::type_exception(node->range.start, error_message.str());
				}

			}
		}

		bool visit(ir::ast::statement::statement_block* node) override
		{
			current_block = node;
			return true;
		}

		bool visit(ir::ast::statement::declaration_block* node) override
		{
			current_block = node;
			return true;
		}
	};

	void type_analyzer::run()
	{
		type_analyzer_visitor visitor;
		root_->visit(&visitor);
	}

	type_analyzer::type_analyzer(ir::ast::statement::declaration_block* root) :
		root_(root)
	{}
}