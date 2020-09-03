#pragma once

#include <cstddef>

namespace seam::core::utils
{
	struct position
	{
		std::size_t line = 0; // line position
		std::size_t column = 0; // column position

		position(const std::size_t line, const std::size_t column) :
			line(line), column(column)
		{}
	};
}