#pragma once

#include <string>

namespace seam {
    enum class SymbolType {
        stNone,
        stType,
        stVariable,
        stFunction,
    };

    class Symbol {
        const SymbolType type_;
        const std::string name_;
    protected:
        explicit Symbol(SymbolType type, std::string name);
    public:
        [[nodiscard]] SymbolType get_type() const;
        [[nodiscard]] std::string get_name() const;
    };
}