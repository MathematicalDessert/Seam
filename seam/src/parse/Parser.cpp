#include "parse/Parser.h"
#include "core/Exception.h"
#include <iostream>

namespace seam {
    Parser::Parser(Module* module)
        : module_(module) {   }

    void Parser::expect(TokenType expected_token_type) {
        if (lexer_->next() != expected_token_type) {
            throw ExpectedTokenNotFound("TODO", SourcePosition { 0, 0 });
        }
    }

    std::string Parser::expect_and_get_value(TokenType expected_token_type) {
        expect(expected_token_type);
        return lexer_->token().get_data();
    }

    std::unique_ptr<ast::statement::FunctionDefinitionStatement> Parser::parse_function_definition_statement() {
        auto function_name = expect_and_get_value(TokenType::tkIdentifier);

        

        expect(TokenType::symbOpenBrace);

        expect(TokenType::symbCloseBrace);
        return nullptr;
    }

    std::unique_ptr<ast::statement::Statement> Parser::parse_statement() {
        std::unique_ptr<ast::statement::Statement> statement;

        switch (lexer_->peek()) {
            case TokenType::kwFunction: {
                lexer_->next(); // discard keyword token
                statement = parse_function_definition_statement();
                break;
            }
            case TokenType::tkEOF: [[fallthrough]];
            default: {
                // TODO: throw exception if statement not found
                break;
            }
        }

        expect(TokenType::symbSeparator);
        return std::move(statement); // TODO: Parse statements.
    }

    std::unique_ptr<ast::Module> Parser::parse_module() {
        auto mod = std::make_unique<ast::Module>();

        while (true) {
            auto next_token = lexer_->peek();

            if (next_token == TokenType::tkEOF) {
                break;
            }

            mod->add_statement(parse_statement());
        }

        return std::move(mod);
    }

    std::unique_ptr<ast::Module> Parser::parse() {
        auto module_source = LexState(module_->get_source());
        lexer_ = std::make_unique<Lexer>(module_source);

        return parse_module();
    }
}
