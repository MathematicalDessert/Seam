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

    std::unique_ptr<ast::FunctionDeclaration> Parser::parse_function_declaration() {
        auto function_name = expect_and_get_value(TokenType::tkIdentifier);

        

        expect(TokenType::symbOpenBrace);

        expect(TokenType::symbCloseBrace);
        return std::make_unique<ast::FunctionDeclaration>(function_name);
    }

    std::unique_ptr<ast::TypeDeclaration> Parser::parse_type_declaration() {
        auto type_name = expect_and_get_value(TokenType::tkIdentifier);

        if (lexer_->peek() == TokenType::symbEqual) {
			// Type Alias
            return std::make_unique<ast::TypeAliasDeclaration>(type_name);
        } else {
        	// TODO: finish types

            expect(TokenType::symbOpenBrace);
        	// TODO: Parse type body
            expect(TokenType::symbCloseBrace);
        	
            return std::make_unique<ast::TypeDeclaration>(type_name);
        }        
    }

	
    std::unique_ptr<ast::statement::Statement> Parser::parse_statement() {
        std::unique_ptr<ast::statement::Statement> statement;

        switch (lexer_->peek()) {
            case TokenType::kwFunction: {
                lexer_->next(); // discard keyword token
                //statement = nullptr;//parse_function_declaration();
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

	std::unique_ptr<ast::Declaration> Parser::parse_declaration() {
        std::unique_ptr<ast::Declaration> declaration;

        switch (lexer_->peek()) {
        case TokenType::kwFunction: {
            lexer_->next(); // discard function token
            declaration = parse_function_declaration();
            break;
        }
        case TokenType::kwType: {
            lexer_->next();
            declaration = parse_type_declaration();
            break;
        }
        default: break;
        }
    	
        return std::move(declaration);
	}

    std::unique_ptr<ast::DeclarationBlock> Parser::parse_declaration_block() {
        auto mod = std::make_unique<ast::DeclarationBlock>();

        while (true) {
            auto next_token = lexer_->peek();

            if (next_token == TokenType::tkEOF) {
                break;
            }

            mod->add_declaration(parse_declaration());
        }

        return std::move(mod);
    }

    std::unique_ptr<ast::DeclarationBlock> Parser::parse() {
        auto module_source = LexState(module_->get_source());
        lexer_ = std::make_unique<Lexer>(module_source);

        return parse_declaration_block();
    }
}
