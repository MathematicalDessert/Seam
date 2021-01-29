#pragma once

#include <memory>
#include <vector>
#include <string_view>

#include "../ASTNode.h"

namespace seam::ast::statement {
    class Statement : ASTNode {  };

    class FunctionDefinitionStatement : public Statement {
        std::string_view name_;
    public:
        explicit FunctionDefinitionStatement(std::string name)
            : name_(name) 
        {}

        std::string_view get_name() const { return name_; }
    };

    using StatementList = std::vector<std::unique_ptr<Statement>>;
}