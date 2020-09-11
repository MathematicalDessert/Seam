#include "graphviz_visitor.h"

namespace seam::core::debug
{
	void graphviz_visitor::write_node(void* id, const std::string& label)
	{
		out_ << 'x' << id << "[label=\"" << label << "\"]\n";
		if (parent_id_)
		{
			out_ << 'x' << parent_id_ << "->x" << id << '\n';
		}
	}
}