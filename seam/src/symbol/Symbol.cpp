#include "symbol/Symbol.h"

namespace seam {
	Symbol::Symbol(const SymbolType type, std::string name)
		: type_(type), name_(std::move(name)) {
		
	}

	SymbolType Symbol::get_type() const {
		return type_;
	}

	std::string Symbol::get_name() const {
		return name_;
	}
}