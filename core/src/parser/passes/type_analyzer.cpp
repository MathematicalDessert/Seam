#include <sstream>

#include "../../utils/exception.hpp"
#include "type_analyzer.hpp"
#include "../../ir/ast/visitor.hpp"
#include "../../types/number_type.hpp"

namespace seam::core::parser::passes
{
	struct type_analyzer_visitor final : ir::ast::visitor
	{
		ir::ast::statement::block* current_block;
		std::shared_ptr<types::base_type> current_type;
		std::shared_ptr<types::base_type> return_type = nullptr;

		bool visit(ir::ast::expression::bool_literal* node) override
		{
			current_type = node->type;
			return false;
		}

		bool visit(ir::ast::expression::number_literal* node) override
		{
			current_type = node->type;
			return false;
		}

		bool visit(ir::ast::expression::string_literal* node) override
		{
			current_type = node->type;
			return false;
		}

		bool visit(ir::ast::expression::variable* node) override
		{
			auto var = current_block->get_variable(node->name);
			
			current_type = var ? var->type : node->type;
			return false;
		}

		bool visit(ir::ast::expression::unary* node) override
		{
			// is unary number operator on non-number
			node->right->visit(this);
			const auto expression_type = current_type;
			
			if ((is_arithmetic_operator(node->operation) && !expression_type->is_number())
				|| (!is_arithmetic_operator(node->operation) && expression_type->is_boolean()))
			{
				std::stringstream error_message;
				error_message << "unary operator "
					<< lexer::lexeme_type_name_array[static_cast<int>(node->operation)]
					<< " is not supported on type '"
					<< expression_type->get_name()
					<< "'";
				throw utils::type_exception(node->range.start, error_message.str());
			}
			
			return false;
		}
		
		bool visit(ir::ast::expression::binary* node) override
		{
			node->left->visit(this);
			const auto lhs_type = current_type;

			node->right->visit(this);
			const auto rhs_type = current_type;


			// both are numbers
			if (is_arithmetic_operator(node->operation))
			{
				if (!lhs_type->is_number() || !rhs_type->is_number())
				{
					std::stringstream error_message;
					error_message << "binary operator "
						<< lexer::lexeme_type_name_array[static_cast<int>(node->operation)]
						<< " is only supported with number types";
					throw utils::type_exception(node->range.start, error_message.str());
				}

				auto lhs_number = static_cast<types::number_type*>(lhs_type.get());
				auto new_type = lhs_number->resize(static_cast<types::number_type*>(rhs_type.get()));

				node->left->type = new_type;
				node->right->type = new_type;
				
				current_type = new_type;
			}
			else if (is_comparision_operator(node->operation))
			{
				// 1 == 2
				// true == false
				// "asd" == "asd"
				//

				if (is_numerical_comparision_operator(node->operation))
				{
					if (!lhs_type->is_number() || !rhs_type->is_number())
					{
						std::stringstream error_message;
						error_message << "binary operator "
							<< lexer::lexeme_type_name_array[static_cast<int>(node->operation)]
							<< " is only supported with number types";
						throw utils::type_exception(node->range.start, error_message.str());
					}
					
					auto lhs_number = static_cast<types::number_type*>(lhs_type.get());
					auto new_type = lhs_number->resize(static_cast<types::number_type*>(rhs_type.get()));

					node->left->type = new_type;
					node->right->type = new_type;
					
					current_type = types::get_base_type_from_name("bool");
					return false;
				}
				
				if (lhs_type != rhs_type)
				{
					std::stringstream error_message;
					error_message << "binary operator "
						<< lexer::lexeme_type_name_array[static_cast<int>(node->operation)]
						<< " is being used on two different types!";
					throw utils::type_exception(node->range.start, error_message.str());
				}
				current_type = types::get_base_type_from_name("bool");
			}
			
			
			return false;
		}

		bool visit(ir::ast::statement::variable_assignment* node) override
		{
			auto variable_type = current_block->get_variable(node->variable_name)->type;
			if (!variable_type) // auto variable
			{
				node->value->visit(this);
				node->type = current_type;

				current_block->get_variable(node->variable_name)->type = current_type;
			}
			else
			{

				// TODO: Use virtual override for EQ operation on types...
				
				// a :i8 = "asd"

				if (node->value)
				{
					node->value->visit(this); // expression type
					if ((!variable_type->is_number() && !current_type->is_number()) && variable_type != current_type)
					{
						std::stringstream error_message;
						error_message << "expression of type "
							<< current_type->get_name()
							<< "cannot be assigned to variable of type "
							<< variable_type->get_name();
						throw utils::type_exception(node->range.start, error_message.str());
					}
				}
			}
			return false;
		}

		bool visit(ir::ast::statement::ret_stat* node) override
		{
			node->value->visit(this);
			if (!return_type) // we're expecting a return type,
							  // if not we should assign this as ret type
			{
				return_type = current_type;
			}
			else
			{
				
				if ((return_type->is_number() && current_type->is_number())
					&& return_type != current_type) // two different numbers
				{
					// cAST THEM!
				}
				else if (return_type != current_type)
				{
					std::stringstream error_message;
					error_message << "return type mismatch: got "
						<< current_type->get_name()
						<< ", expected "
						<< return_type->get_name();
					throw utils::type_exception(node->range.start, error_message.str());
				}
			}

			return false;
		}
		
		bool visit(ir::ast::statement::function_definition* node) override
		{
			return_type = node->return_type;

			const auto old_block = current_block;
			current_block = node->block.get();
			for (const auto& declaration : node->block->body)
			{
				declaration->visit(this);
			}

			node->return_type = return_type;
			
			return_type = nullptr;

			current_block = old_block;
			
			return false;
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
