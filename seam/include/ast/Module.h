#pragma once

#include "ASTNode.h"
#include "statement/Statement.h"

namespace seam::ast {
    class Module : ASTNode {
        statement::StatementList body_;
    public:
        statement::StatementList& get_body();
        void add_statement(std::unique_ptr<statement::Statement> statement);
    };
}