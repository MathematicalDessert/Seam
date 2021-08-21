#include "parser/parser.h"

#include "exception.h"

namespace seam {
	std::unique_ptr<ast::FunctionDeclaration> Parser::parse_function_declaration() {
		expect<SymbolType::KeywordFn>();

		const auto func_name = consume_token<SymbolType::Identifier, std::wstring>();

		return std::make_unique<ast::FunctionDeclaration>(ast::FunctionDeclaration{
			func_name,
			parse_statement_list(),
		});
	}

	ast::DeclarationList Parser::parse_declaration_list() {
		ast::DeclarationList body;

		while (true) {
			switch (lexer_->peek()) {
			case SymbolType::KeywordFn: {
				body.emplace_back(parse_function_declaration());
				break;
			}
			case SymbolType::KeywordType: {
				break;
			}
			case SymbolType::None: {
				return body;
			}
			default: {
				break;
			}
			}
			break;
		}
	}


	Parser::Parser(std::unique_ptr<Lexer> lexer)
		: lexer_(std::move(lexer)) {
		
	}

	std::unique_ptr<ast::Program> Parser::parse() {
		if (!lexer_) {
			throw SeamException(L"no lexer found!"); // throw proper exception
		}

		return std::make_unique<ast::Program>(ast::Program{
			parse_declaration_list()
		});
	}
}
