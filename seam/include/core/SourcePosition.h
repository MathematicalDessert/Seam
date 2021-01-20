#pragma once

#include <cmath>

namespace seam {
	/**
	 * The SourcePosition struct is a simple container for
	 * a line and column index.
	 */
	struct SourcePosition {
		const size_t line;
		const size_t column;

		SourcePosition(const size_t line, const size_t column)
			: line(line), column(column) {}
	};
}