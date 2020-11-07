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

	bool_literal::bool_literal(const utils::position_range range, const bool value) :
		literal(range), value(value)
	{
		type = types::get_base_type_from_name("bool", false);
	}
;
	
	void bool_literal::visit(visitor* vst)
	{
		vst->visit(this);
	}

	string_literal::string_literal(const utils::position_range range, std::string value) :
		literal(range), value(std::move(value))
	{
		type = types::get_base_type_from_name("string", false);
	}
	
	void string_literal::visit(visitor* vst)
	{
		vst->visit(this);
	}

	number_literal::number_literal(const utils::position_range range, const std::string& value) :
		literal(range)
	{
		if (value.find('.') != std::string::npos)
		{
			this->value = std::stod(value);
			type = types::get_base_type_from_name("f64", false);
		}
		else
		{
			this->value = std::stoull(value);
			this->is_unsigned = value[0] != '-';
			type = types::get_smallest_viable_number_type(this->value, this->is_unsigned);
		}
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
