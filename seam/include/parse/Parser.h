#pragma once

#include "Lexer.h"
#include "ast/Module.h"
#include "ast/Declaration.h"
#include "ast/statement/Statement.h"
#include "core/Module.h"

namespace seam {
	class Parser {
		Module *module_;
		std::unique_ptr<Lexer> lexer_;

		void expect(TokenType expected_token_type);
		std::string expect_and_get_value(TokenType expected_token_type);

		std::unique_ptr<ast::FunctionDeclaration> parse_function_declaration();
		std::unique_ptr<ast::TypeDeclaration> parse_type_declaration();
		 
		std::unique_ptr<ast::statement::Statement> parse_statement();
		std::unique_ptr<ast::Declaration> parse_declaration();
		std::unique_ptr<ast::DeclarationBlock> parse_declaration_block();
	public:
		explicit Parser(Module *module);

		std::unique_ptr<ast::DeclarationBlock> parse();
	};
} // namespace seam