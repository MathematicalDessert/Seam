#include "expression.hpp"
#include "visitor.hpp"

namespace seam::core::ir::ast::expression
{
	void unary::visit(visitor* vst)
	{
		if (vst->visit(this))
		{
			right->visit(vst);
		}
	}

	void binary::visit(visitor* vst)
	{
		if (vst->visit(this))
		{
			left->visit(vst);
			right->visit(vst);
		}
	}
	
	void bool_literal::visit(visitor* vst)
	{
		vst->visit(this);
	}

	void string_literal::visit(visitor* vst)
	{
		vst->visit(this);
	}

	void number_literal::visit(visitor* vst)
	{
		vst->visit(this);
	}

	void call::visit(visitor* vst)
	{
		if (vst->visit(this))
		{
			for (const auto& argument : arguments)
			{
				argument->visit(vst);
			}
		}		
	}

	void variable::visit(visitor* vst)
	{
		vst->visit(this);
	}

}