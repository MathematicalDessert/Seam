#include "statement.hpp"
#include "visitor.hpp"

namespace seam::core::ir::ast::statement
{
	std::shared_ptr<ir::ast::variable> block::get_variable(const std::string& variable_name)
	{
		auto* current_block = this;
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

	void statement_block::visit(visitor* vst)
	{
		if (vst->visit(this))
		{
			for (const auto& statement : body)
			{
				statement->visit(vst);
			}
		}
	}
	
	void declaration_block::visit(visitor* vst)
	{
		if (vst->visit(this))
		{
			for (const auto& declaration : body)
			{
				declaration->visit(vst);
			}
		}
	}

	void function_definition::visit(visitor* vst)
	{
		if (vst->visit(this))
		{
			block->visit(vst);
		}
	}

	void type_alias_declaration::visit(visitor* vst)
	{
		vst->visit(this);
	}

	void variable_declaration::visit(visitor* vst)
	{
		if (vst->visit(this))
		{
			value->visit(vst);
		}
	}

	void type_class_definition::visit(visitor* vst)
	{
		if (vst->visit(this))
		{
			body->visit(vst);
		}
	}

	void variable_assignment::visit(visitor* vst)
	{
		if (vst->visit(this))
		{
			value->visit(vst);
		}
	}
	
	void ret_stat::visit(visitor* vst)
	{
		if (vst->visit(this))
		{
			value->visit(vst);
		}
	}

	void while_stat::visit(visitor* vst)
	{
		if (vst->visit(this))
		{
			condition->visit(vst);
			body->visit(vst);
		}
	}

	void if_stat::visit(visitor* vst)
	{
		if (vst->visit(this))
		{
			condition->visit(vst);
			body->visit(vst);

			if (else_body)
			{
				else_body->visit(vst);
			}
		}
	}
}
