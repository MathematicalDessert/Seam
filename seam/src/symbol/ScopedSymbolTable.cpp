#include "symbol/ScopedSymbolTable.h"

namespace seam {
    bool ScopedSymbolTable::symbol_exists(const std::string& symbol_name, const bool search_all_scopes) {
        auto scope = this;

        while (scope) {
            if (auto symbol = scope->symbols_.find(symbol_name); symbol != scope->symbols_.cend()) {
                return true;
            }

            if (search_all_scopes) {
                scope = scope->parent_.get();
            }
        }

        return false;
    }

    Symbol* ScopedSymbolTable::get_symbol(const std::string& symbol_name, const bool search_all_scopes) {
        auto scope = this;

        while (scope) {
            if (auto symbol = scope->symbols_.find(symbol_name); symbol != scope->symbols_.cend()) {
                return symbol->second;
            }

            if (search_all_scopes) {
                scope = scope->parent_.get();
            }
        }

        std::stringstream error_message;
        error_message << "symbol '" << symbol_name << "' not found";

        throw SymbolNotFound(error_message.str(), { 0, 0 }); // TODO: proper errors!
    }
}