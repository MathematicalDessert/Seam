#include "statement.hpp"
#include "visitor.hpp"

namespace seam::core::ir::ast::statement
{
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
			else_body->visit(vst);
		}
	}
}
