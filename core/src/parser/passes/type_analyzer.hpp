#pragma once

#include "../../ir/ast/statement.hpp"

namespace seam::core::parser::passes
{
	class type_analyzer
	{
		ir::ast::statement::declaration_block* root_;
	public:
		void run();

		explicit type_analyzer(ir::ast::statement::declaration_block* root);
	};
}