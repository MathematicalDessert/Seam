#include "parser/parser.h"
#include "exception.h"
#include <ast/print_visitor.h>

namespace seam {
	bool is_unary_operator(const SymbolType symbol) {
		switch (symbol) {
			case SymbolType::OpSub: {
				return true;
			}
			default: return false;
		}
	}

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

	std::unique_ptr<ast::expression::Expression> Parser::parse_primary_expression() {
		switch (lexer_->peek()) {
			case SymbolType::SymbOpenParen: {
				lexer_->next();
				auto expr = parse_primary_expression();
				expect<SymbolType::SymbCloseParen>();
				return std::move(expr);
			}
			case SymbolType::StringLiteral: {
				return std::make_unique<ast::expression::StringLiteral>(consume_token<SymbolType::StringLiteral, std::wstring>());
			}
			case SymbolType::NumberLiteral: {
				return std::make_unique<ast::expression::NumberLiteral>(consume_token<SymbolType::NumberLiteral, std::wstring>());
			}
			case SymbolType::KeywordTrue:
			case SymbolType::KeywordFalse: {
				return std::make_unique<ast::expression::BooleanLiteral>(lexer_->next()->type == SymbolType::KeywordTrue);
			}
			default:break;
		}
		return nullptr;
	}

	std::unique_ptr<ast::expression::Expression> Parser::parse_expression() {
		// TODO: Check is unary operator & add operator precedence

		std::unique_ptr<ast::expression::Expression> expr;
		if constexpr (true) {
			expr = parse_primary_expression();
		}

		// TODO: add
		return std::move(expr);
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

		auto expr = parse_primary_expression();

		return std::make_unique<ast::statement::LetStatement>(var_name, type, std::move(expr));
	}

	std::unique_ptr<ast::statement::WhileStatement> Parser::parse_while_statement() {
		lexer_->next();

		expect<SymbolType::SymbOpenParen>();
		auto expr = parse_expression();
		expect<SymbolType::SymbCloseParen>();

		auto body = parse_statement_block();

		return std::make_unique<ast::statement::WhileStatement>(
			std::move(expr),
			std::move(body));
	}

	std::unique_ptr<ast::statement::IfStatement> Parser::parse_if_statement() {
		lexer_->next(); // consume if keyword

		expect<SymbolType::SymbOpenParen>();
		auto expr = parse_expression();
		expect<SymbolType::SymbCloseParen>();

		auto if_body = parse_statement_block();
		std::unique_ptr<ast::statement::StatementBlock> else_body;

		if (lexer_->peek() == SymbolType::KeywordElseIf) {
			auto inner_if = parse_if_statement();
			ast::statement::StatementList list;
			list.emplace_back(std::move(inner_if));

			else_body = std::make_unique<ast::statement::StatementBlock>(std::move(list));
		} else if (lexer_->peek() == SymbolType::KeywordElse) {
			lexer_->next();
			else_body = parse_statement_block();
		}

		return std::make_unique<ast::statement::IfStatement>(
			std::move(expr),
			std::move(if_body),
			std::move(else_body));
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
			case SymbolType::KeywordIf: {
				return parse_if_statement();
			}
			case SymbolType::KeywordWhile: {
				return parse_while_statement();
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
