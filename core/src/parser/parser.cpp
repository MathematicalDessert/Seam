#include "parser/parser.h"
#include "exception.h"
#include <ast/print_visitor.h>

namespace seam {
	std::wstring Parser::try_parse_type() {
		expect<SymbolType::Colon>();
		return consume_token<SymbolType::Identifier, std::wstring>();
	}

	ast::ParameterList Parser::parse_parameter_list() {
		ast::ParameterList params;

		expect<SymbolType::SymbOpenParen>();

		while (lexer_->peek() == SymbolType::Identifier) {
			const auto param_name = consume_token<SymbolType::Identifier, std::wstring>();
			expect<SymbolType::Colon>();
			const auto param_type = consume_token<SymbolType::Identifier, std::wstring>();

			params.emplace_back(ast::Parameter {
				param_name,
				param_type
			});
		}

		expect<SymbolType::SymbCloseParen>();
		return params;
	}

	// TODO: add extra information to error exceptions

	std::unique_ptr<ast::expression::Expression> Parser::parse_expression() {
		switch (lexer_->peek()) {
		case SymbolType::SymbOpenParen: {
			lexer_->next();
			auto expr = parse_expression();
			expect<SymbolType::SymbCloseParen>();
			return std::move(expr);
		}
		case SymbolType::StringLiteral: {
			return std::make_unique<ast::expression::StringLiteral>(consume_token<SymbolType::StringLiteral, std::wstring>());
		}
		case SymbolType::NumberLiteral: {
			return std::make_unique<ast::expression::NumberLiteral>(consume_token<SymbolType::StringLiteral, std::wstring>());
		}
		default:break;
		}
		return nullptr;
	}

	std::unique_ptr<ast::statement::LetStatement> Parser::parse_let_statement() {
		const auto var_name = consume_token<SymbolType::Identifier, std::wstring>();

		// is type
		std::wstring type;
		switch (lexer_->peek()) {
			case SymbolType::Colon: {
				type = try_parse_type();
				expect<SymbolType::OpAssign>();
				break;
			}
			case SymbolType::ColonEquals: {
				lexer_->next();
				break;
			}
			default: {
				// TODO: throw exception
				break;
			}
		}

		auto expr = parse_expression();

		return std::make_unique<ast::statement::LetStatement>(var_name, type, std::move(expr));
	}

	std::unique_ptr<ast::statement::Statement> Parser::parse_statement() {
		switch (lexer_->peek()) {
			case SymbolType::KeywordLet: {
				lexer_->next();
				return parse_let_statement();
			}
			case SymbolType::SymbOpenBrace: {
				return parse_statement_block();
			}
			default: {
				return nullptr; // TODO: Set this
			}
		}
	}

	std::unique_ptr<ast::statement::StatementBlock> Parser::parse_statement_block() {
		ast::statement::StatementList body;

		expect<SymbolType::SymbOpenBrace>();

		while (true) {
			auto statement = parse_statement();
			if (!statement) {
				break;
			}

			body.emplace_back(std::move(statement));
		}

		expect<SymbolType::SymbCloseBrace>();

		return std::make_unique<ast::statement::StatementBlock>(std::move(body));
	}

	std::unique_ptr<ast::FunctionDeclaration> Parser::parse_function_declaration() {
		const auto func_name = consume_token<SymbolType::Identifier, std::wstring>();
		const auto param_list = parse_parameter_list();

		std::wstring return_type;
		if (lexer_->peek() == SymbolType::Arrow) {
			lexer_->next();
			return_type = consume_token<SymbolType::Identifier, std::wstring>();
		}

		auto body = parse_statement_block();
		return std::make_unique<ast::FunctionDeclaration>(func_name, param_list, return_type, std::move(body));
	}

	ast::DeclarationList Parser::parse_declaration_list() {
		ast::DeclarationList body;

		while (true) {
			switch (lexer_->peek()) {
			case SymbolType::KeywordFn: {
				lexer_->next(); // TODO: find better way of discarding...
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
