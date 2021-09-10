#include "parser/parser.h"

#include "exception.h"
#include <ast/print_visitor.h>
#include <unordered_map>

namespace {
	struct EnumClassHash {
		template<typename T>
		size_t operator()(T t) const {
			return static_cast<size_t>(t);
		}
	};
	template <typename Key>
	using HashType = typename std::conditional<std::is_enum_v<Key>, EnumClassHash, std::hash<Key>>::type;
}

namespace seam {
	bool is_unary_operator(const TokenType symbol) {
		switch (symbol) {
			case TokenType::OpSub: {
				return true;
			}
			default: return false;
		}
	}

	bool is_binary_operator(const TokenType type) {
		switch (type) {
			case TokenType::OpEq:
			case TokenType::OpSub:
			case TokenType::OpMul:
			case TokenType::OpAdd: {
				return true;
			}
			default: return false;
		}
	}

	bool is_right_assoc(const TokenType type) {
		switch (type) {
			case TokenType::OpMul:
			case TokenType::OpAdd: {
				return true;
			}
			default: return false;
		}
	}

	bool is_left_assoc(const TokenType type) {
		switch (type) {
			case TokenType::OpMul:
			case TokenType::OpAdd: {
				return true;
			}
			default: return false;
		}
	}

	std::wstring Parser::try_parse_type() {
		expect<TokenType::Colon>();
		return consume_token<TokenType::Identifier, std::wstring>();
	}

	ast::ParameterList Parser::parse_parameter_list() {
		ast::ParameterList params;

		expect<TokenType::OpenParen>();

		while (lexer_->peek() == TokenType::Identifier) {
			const auto param_name = consume_token<TokenType::Identifier, std::wstring>();
			expect<TokenType::Colon>();
			const auto param_type = consume_token<TokenType::Identifier, std::wstring>();

			params.emplace_back(ast::Parameter {
				param_name,
				param_type
			});
		}

		expect<TokenType::CloseParen>();
		return params;
	}

	ast::expression::ExpressionList Parser::parse_arg_list() {
		ast::expression::ExpressionList args;

		expect<TokenType::OpenParen>();

		if (lexer_->peek() != TokenType::CloseParen) {
			args.emplace_back(parse_expression());
		}

		while (lexer_->peek() == TokenType::Comma) {
			discard();
			args.emplace_back(parse_expression());
		}

		expect<TokenType::CloseParen>();
		return std::move(args);
	}


	// TODO: add extra information to error exceptions

	std::unique_ptr<ast::expression::Expression> Parser::parse_primary_expression() {
		switch (lexer_->peek()) {
			case TokenType::OpenParen: {
                discard();
				auto expr = parse_expression();

				expect<TokenType::CloseParen>();
				return std::move(expr);
			}
			case TokenType::Identifier: {
				return std::make_unique<ast::expression::Identifier>(consume_token<TokenType::Identifier, std::wstring>());
			}
			case TokenType::StringLiteral: {
				return std::make_unique<ast::expression::StringLiteral>(consume_token<TokenType::StringLiteral, std::wstring>());
			}
			case TokenType::NumberLiteral: {
				return std::make_unique<ast::expression::NumberLiteral>(consume_token<TokenType::NumberLiteral, std::wstring>());
			}
			case TokenType::KeywordTrue:
			case TokenType::KeywordFalse: {
				return std::make_unique<ast::expression::BooleanLiteral>(lexer_->next()->type == TokenType::KeywordTrue);
			}
			default:break;
		}
		return nullptr;
	}

	const std::unordered_map<TokenType, size_t, HashType<TokenType>> binary_priority {
		{ TokenType::OpEq, 5 },
        { TokenType::OpAdd, 6 }, { TokenType::OpSub, 6 },
		{ TokenType::OpMul, 7 }, { TokenType::OpDiv, 7 },
	};
	auto get_binary_priority(const TokenType type) {
		auto search = binary_priority.find(type);

		return search != binary_priority.end() ? search->second : -1;
	}

	std::unique_ptr<ast::expression::Expression> Parser::parse_expression(std::unique_ptr<ast::expression::Expression> expr, const size_t right_binding_power) {
		// TODO: Check is unary operator & add operator precedence

		auto next_token = lexer_->peek();
		while (is_binary_operator(next_token) && get_binary_priority(next_token) >= right_binding_power) {
			auto operator_token = lexer_->next();
			auto rhs = parse_primary_expression();

			next_token = lexer_->peek();
			while (is_binary_operator(next_token) 
				&& (get_binary_priority(next_token) > get_binary_priority(operator_token->type)) 
					|| (is_right_assoc(next_token) && get_binary_priority(next_token) == get_binary_priority(operator_token->type))) {
					rhs = parse_expression(std::move(rhs), get_binary_priority(operator_token->type));
					next_token = lexer_->peek();
			}

			expr = std::make_unique<ast::expression::BinaryExpression>(operator_token->type, 
				std::move(expr), std::move(rhs));
		}
		return std::move(expr);
	}

	std::unique_ptr<ast::expression::Expression> Parser::parse_expression() {
		if (is_unary_operator(lexer_->peek())) {
			auto op = lexer_->next()->type;
			auto expr = parse_expression();

			return std::make_unique<ast::expression::UnaryExpression>(
				op,
				std::move(expr));
		}

		const auto shrouded_expression = lexer_->peek() == TokenType::OpenParen;
		auto expr = parse_primary_expression();

		switch (lexer_->peek()) {
			case TokenType::OpenParen: {
				if (dynamic_cast<ast::expression::Identifier*>(expr.get()) || shrouded_expression) {
					auto arg_list = parse_arg_list();
					expr = std::make_unique<ast::expression::FunctionCall>(
						std::move(expr),
						std::move(arg_list)
						);
				}
				break;
			}
			case TokenType::OpDecrement:
			case TokenType::OpIncrement: {
				auto op = lexer_->next()->type;
				expr = std::make_unique<ast::expression::PostfixExpression>(
					op,
					std::move(expr));
				break;
			}
			default: break;
		}

		return parse_expression(std::move(expr));
	}


	std::unique_ptr<ast::statement::LetStatement> Parser::parse_let_statement() {
		const auto var_name = consume_token<TokenType::Identifier, std::wstring>();

		// is type
		std::wstring type;
		switch (lexer_->peek()) {
			case TokenType::Colon: {
				type = try_parse_type();
				expect<TokenType::OpAssign>();
				break;
			}
			case TokenType::ColonEquals: {
                discard();
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

	std::unique_ptr<ast::statement::WhileStatement> Parser::parse_while_statement() {
		lexer_->next();

		expect<TokenType::OpenParen>();
		auto expr = parse_expression();
		expect<TokenType::CloseParen>();

		auto body = parse_statement_block();

		return std::make_unique<ast::statement::WhileStatement>(
			std::move(expr),
			std::move(body));
	}

	std::unique_ptr<ast::statement::IfStatement> Parser::parse_if_statement() {
		lexer_->next(); // consume if keyword

		expect<TokenType::OpenParen>();
		auto expr = parse_expression();
		expect<TokenType::CloseParen>();

		auto if_body = parse_statement_block();
		std::unique_ptr<ast::statement::StatementBlock> else_body;

		if (lexer_->peek() == TokenType::KeywordElseIf) {
			auto inner_if = parse_if_statement();
			ast::statement::StatementList list;
			list.emplace_back(std::move(inner_if));

			else_body = std::make_unique<ast::statement::StatementBlock>(std::move(list));
		} else if (lexer_->peek() == TokenType::KeywordElse) {
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
			case TokenType::KeywordLet: {
				lexer_->next();
				return parse_let_statement();
			}
			case TokenType::OpenBrace: {
				return parse_statement_block();
			}
			case TokenType::KeywordIf: {
				return parse_if_statement();
			}
			case TokenType::KeywordWhile: {
				return parse_while_statement();
			}
			default: {
				auto expression = parse_expression();

				if (dynamic_cast<ast::expression::FunctionCall*>(expression.get())) {
					return std::make_unique<ast::statement::LetStatement>(
						L"<DISCARD>",
						L"<DISCARD>",
						std::move(expression));
				}

				if (expression) {
					throw generate_exception<ParserException>(
						lexer_->next()->position,
						L"expected statement, got expression");
				}
				return nullptr; // TODO: Set this
			}
		}
	}

	std::unique_ptr<ast::statement::StatementBlock> Parser::parse_statement_block() {
		ast::statement::StatementList body;

		expect<TokenType::OpenBrace>();

		while (true) {
			auto statement = parse_statement();
			if (!statement) {
				break;
			}

			body.emplace_back(std::move(statement));
		}

		expect<TokenType::CloseBrace>();

		return std::make_unique<ast::statement::StatementBlock>(std::move(body));
	}

	std::unique_ptr<ast::FunctionDeclaration> Parser::parse_function_declaration() {
		const auto func_name = consume_token<TokenType::Identifier, std::wstring>();
		const auto param_list = parse_parameter_list();

		std::wstring return_type;
		if (lexer_->peek() == TokenType::Arrow) {
			lexer_->next();
			return_type = consume_token<TokenType::Identifier, std::wstring>();
		}

		auto body = parse_statement_block();
		return std::make_unique<ast::FunctionDeclaration>(func_name, param_list, return_type, std::move(body));
	}

    std::unique_ptr<ast::Declaration> Parser::parse_type_decl() {
	    auto name = consume_token<TokenType::Identifier, std::wstring>();

	    std::unique_ptr<ast::Declaration> decl;
	    switch (lexer_->peek()) {
	        case TokenType::OpAssign: {
	            expect<TokenType::OpAssign>();
	            auto type = consume_token<TokenType::Identifier, std::wstring>();
	            decl = std::make_unique<ast::TypeAliasDeclaration>(
	                    std::move(name),
	                    std::move(type)
	                    );
	            break;
	        };
	        case TokenType::OpenBrace: {
	            expect<TokenType::OpenBrace>();
	            auto body = parse_declaration_list();

	            decl = std::make_unique<ast::TypeDeclaration>(
	                    std::move(name),
	                    std::move(body)
	                    );
	            break;
	        };
	        default: break;
	    }

	    return std::move(decl);
	}

	ast::DeclarationList Parser::parse_declaration_list() {
		ast::DeclarationList body;

		while (true) {
			switch (lexer_->peek()) {
				case TokenType::KeywordFn: {
                    discard(); // TODO: find better way of discarding...
					body.emplace_back(parse_function_declaration());
					break;
				}
				case TokenType::KeywordType: {
                    discard();
				    body.emplace_back(parse_type_decl());
					break;
				}
				case TokenType::None: {
					return body;
				}
				default: {
					auto token = lexer_->next();
					throw generate_exception<ParserException>(
                            token->position,
                            L"expected declaration, got {}",
                            token_type_to_name(token->type)
					);
				}
			}
		}
	}

	Parser::Parser(std::unique_ptr<Lexer> lexer)
		: lexer_(std::move(lexer)) { }

	std::unique_ptr<ast::Program> Parser::parse() {
		if (!lexer_) {
		    // This should actually never happen, so?
			throw SeamException(L"no lexer found!"); // throw proper exception
		}

		return std::make_unique<ast::Program>(ast::Program{
			parse_declaration_list()
		});
	}
}
