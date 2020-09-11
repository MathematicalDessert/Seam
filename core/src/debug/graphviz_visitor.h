#pragma once

#include <string>
#include <sstream>

#include "../ir/ast/visitor.hpp"

namespace seam::core::debug
{
	struct graphviz_visitor : ir::ast::visitor
	{
		// TODO: visit methods here!
	private:
		std::stringstream out_;

		void* parent_id_ = nullptr;
		void write_node(void* id, const std::string& label);
	};
}
