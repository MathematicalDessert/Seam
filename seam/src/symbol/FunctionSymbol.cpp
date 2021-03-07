#include "symbol/FunctionSymbol.h"

namespace seam {
	FunctionSymbol::FunctionSymbol(std::string mangled_name)
		: Symbol(SymbolType::stFunction, std::move(mangled_name)) {
		
	}
}