#pragma once

#include <stdexcept>
#include "helper/Format.h"
#include "SourcePosition.h"

namespace seam {
	class CompilerException : public std::runtime_error {
		SourcePosition position_;
	public:
		explicit CompilerException(const std::string& reason, SourcePosition position = { 0, 0 })
			: std::runtime_error(reason), position_(position) {}

		SourcePosition get_position() const { return position_; }
	};

	struct SourceLoadException final : CompilerException {
		explicit SourceLoadException(const std::string& reason) 
			: CompilerException(reason) {}
	};

	struct LexicalException final : CompilerException {
		explicit LexicalException(const std::string& reason, SourcePosition position)
			: CompilerException(reason, position) {}
	};
}
