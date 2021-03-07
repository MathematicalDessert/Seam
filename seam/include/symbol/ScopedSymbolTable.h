#pragma once

#include "Symbol.h"
#include "core/Exception.h"

#include <memory>
#include <sstream>
#include <type_traits>
#include <unordered_map>

namespace seam {
    class ScopedSymbolTable {
        std::string scope_identifier_;
        std::unique_ptr<ScopedSymbolTable> parent_;
        std::unordered_map<std::string, Symbol*> symbols_;

        bool symbol_exists(const std::string& symbol, bool serach_all_scopes = true);
    public:
        explicit ScopedSymbolTable(std::string identifier);
        explicit ScopedSymbolTable(std::string identifier, std::unique_ptr<ScopedSymbolTable> parent);
    	
        /**
         * Returns symbol or throws exception if symbol is not found.
         * 
         * @param symbol_name symbol name to be found.
         * @throws 
         * @returns symbol 
         */
        Symbol* get_symbol(const std::string& symbol_name, bool serach_all_scopes = true);

        template<typename T>
        T* get_symbol_of_type(const std::string& symbol_name) {
            static_assert(std::is_base_of<Symbol, T>::value, "T must inherit from seam::Symbol");

            auto symbol = get_symbol(symbol_name);
            return static_cast<T*>(symbol);
        }

        template<typename T, typename... Args>
        T* add_symbol(std::string symbol_name, Args... args) {
            static_assert(std::is_base_of<Symbol, T>::value, "T must inherit from seam::Symbol");

            if (symbol_exists(symbol_name)) {
                // TODO: throw exception
                return nullptr;
            }

            auto new_symbol = std::make_unique<T>(args...);
            symbols_.emplace(symbol_name, std::move(new_symbol));

            return new_symbol.get();
        }
    };
}