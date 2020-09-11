#pragma once

#include "../../utils/position.hpp"

namespace seam::core::ir::ast
{
	struct visitor;
	struct node
	{
		utils::position_range range;
		
		virtual void visit(visitor* vis) = 0;

		virtual ~node() = default;
	protected:
		explicit node(const utils::position_range range)
			: range(range) {}
	};
}
