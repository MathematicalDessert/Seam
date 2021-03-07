#include "Symbol.h"

namespace seam {
	class FunctionSymbol : public Symbol {
	public:
		explicit FunctionSymbol(std::string mangled_name);
	};
}