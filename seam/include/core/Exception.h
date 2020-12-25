#pragma once

#include <stdexcept>

namespace seam {
	struct SourceLoadException final : std::runtime_error {
		explicit SourceLoadException(const std::string& reason) : std::runtime_error(reason) {}
	};
}
