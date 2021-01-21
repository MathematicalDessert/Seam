#include "ast/Module.h"

namespace seam::ast {
    statement::StatementList& Module::get_body() {
        return body_;
    }

    void Module::add_statement(std::unique_ptr<statement::Statement> statement) {
        body_.push_back(std::move(statement));
    }
}