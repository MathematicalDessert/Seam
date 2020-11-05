#include "statement.hpp"
#include "visitor.hpp"

namespace seam::core::ir::ast::statement
{
	void block::add_symbol(std::string symbol_name, std::unique_ptr<symbol> new_symbol)
	{
		symbols.emplace(symbol_name, std::move(new_symbol));
	}

	symbol* block::get_symbol(const std::string& symbol)
	{
		auto* current_block = this;
		while (current_block)
		{
			if (auto iterator = current_block->symbols.find(symbol); iterator != current_block->symbols.cend())
			{
				return iterator->second.get();
			}
			current_block = current_block->parent;
		}

		return nullptr;
	}
	
	std::shared_ptr<types::base_type> block::get_type(const std::string& type_name)
	{
		const auto symb = get_symbol(type_name);

		if (!symb || !symb->is_type())
		{
			return types::get_base_type_from_name(type_name);
		}

		return symb->get<types::base_type>();
	}

	
	std::shared_ptr<variable> block::get_variable(const std::string& variable_name)
	{
		const auto symb = get_symbol(variable_name);

		if (!symb || !symb->is_var())
		{
			return nullptr;
		}

		return symb->get<variable>();
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
