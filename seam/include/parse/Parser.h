#pragma once

#include "Lexer.h"
#include "ast/Module.h"
#include "ast/statement/Statement.h"
#include "core/Module.h"

namespace seam {
    class Parser {
        Module* module_;

        std::unique_ptr<ast::statement::Statement> parse_statement(Lexer& lexer);
        std::unique_ptr<ast::Module> parse_module(Lexer& lexer);
    public:
        explicit Parser(Module* module);

        std::unique_ptr<ast::Module> parse();
    };
}