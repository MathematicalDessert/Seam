#pragma once

#include "node.hpp"
#include "statement.hpp"
#include "expression.hpp"

#define VISITOR(base, child) virtual bool visit(child* c_node) { return visit(static_cast<base*>(c_node)); }

namespace seam::core::ir::ast
{
	struct visitor
	{
		virtual ~visitor() = default;

		// Base visit
		virtual bool visit(node* node) { return true; }

		// Other visit methods
		VISITOR(node, statement::statement);
		VISITOR(node, statement::statement_block);
		VISITOR(node, statement::declaration_block);
		
		VISITOR(statement::statement, statement::declaration);
		
		VISITOR(statement::declaration, statement::function_declaration);
		VISITOR(statement::function_declaration, statement::function_definition);

		VISITOR(statement::statement, statement::variable_declaration);
		VISITOR(statement::statement, statement::variable_assignment);
		
		VISITOR(statement::statement, statement::ret_stat);
		
		VISITOR(statement::statement, expression::expression);
		VISITOR(expression::expression, expression::unary);
		VISITOR(expression::expression, expression::binary);
		
		VISITOR(expression::expression, expression::literal);
		VISITOR(expression::literal, expression::bool_literal);
		VISITOR(expression::literal, expression::string_literal);
		VISITOR(expression::literal, expression::number_literal);	
	};
}

#undef VISITOR