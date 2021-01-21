#include "parse/Parser.h"

namespace seam {
    Parser::Parser(Module* module)
        : module_(module) {   }

    std::unique_ptr<ast::statement::Statement> Parser::parse_statement(Lexer& lexer) {
        return nullptr; // TODO: Parse statements.
    }

    std::unique_ptr<ast::Module> Parser::parse_module(Lexer& lexer) {
        auto mod = std::make_unique<ast::Module>();

        while (true) {
            auto next_token = lexer.peek();

            if (next_token == TokenType::tkEOF) {
                break;
            }

            mod->add_statement(parse_statement(lexer));
        }

        return std::move(mod);
    }

    std::unique_ptr<ast::Module> Parser::parse() {
        auto module_source = module_->get_lex_state();
        Lexer lexer = Lexer(module_source);

        return parse_module(lexer);
    }
}