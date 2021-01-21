#pragma once

#include <memory>
#include <vector>

#include "../ASTNode.h"

namespace seam::ast::expression {
    class Expression : ASTNode {  };

    class Literal : Expression {  };

    class NumberLiteral : Literal {
        std::uint64_t value_;
    public:
        explicit NumberLiteral(std::uint64_t value)
            : value_(value) {}

        std::uint64_t get_value() { return value_; }
    };

    using ExpressionList = std::vector<std::unique_ptr<Expression>>;
}