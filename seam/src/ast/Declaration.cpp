#include "ast/Declaration.h"

namespace seam::ast {
	void DeclarationBlock::add_declaration(std::unique_ptr<Declaration> declaration) {
		declarations_.push_back(std::move(declaration));
	}
}