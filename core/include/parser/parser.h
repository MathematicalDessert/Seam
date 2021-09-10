#pragma once

#include <ast/ast.h>
#include "lexer.h"

namespace seam {
	class Parser {
		std::unique_ptr<Lexer> lexer_;
		size_t last_binding_power_ = 0;
		
		template <TokenType T>
		void expect(const bool consume = true) const {

			if (const TokenType type = lexer_->peek(); T != type) {
				const auto token = lexer_->next();

				constexpr auto symb_name = token_type_to_name_cexpr<T>();
				throw generate_exception<ParserException>(
                        token->position,
                        L"expected {}, got {}",
                        symb_name,
                        token_type_to_name(type)
					);
			}

			if (consume) {
				lexer_->next();
			}
		}

		template <TokenType TT, typename T>
		[[nodiscard]] auto consume_token() const {
			expect<TT>(false);

			const auto token = lexer_->next();
			if constexpr (std::is_same_v<T, std::wstring>) {
				return token->lexeme;
			} else if constexpr (std::is_same_v<T, double>) {
				return std::stod(token->lexeme);
			}

			if constexpr (std::is_same_v<T, int>) {
				return 1; // TODO: parse numbers
			}
		}

		void discard() const { lexer_->next(); }

		std::wstring try_parse_type();
		ast::ParameterList parse_parameter_list();

		ast::expression::ExpressionList parse_arg_list();

		std::unique_ptr<ast::expression::Expression> parse_primary_expression();
		std::unique_ptr<ast::expression::Expression> parse_expression(std::unique_ptr<ast::expression::Expression> expr, size_t right_binding_power = 0);
		std::unique_ptr<ast::expression::Expression> parse_expression();

		std::unique_ptr<ast::statement::WhileStatement> parse_while_statement();
		std::unique_ptr<ast::statement::IfStatement> parse_if_statement();
		std::unique_ptr<ast::statement::Statement> parse_statement();

		std::unique_ptr<ast::statement::LetStatement> parse_let_statement();
		std::unique_ptr<ast::statement::StatementBlock> parse_statement_block();

		std::unique_ptr<ast::FunctionDeclaration> parse_function_declaration();
		std::unique_ptr<ast::Declaration> parse_type_decl();

		ast::DeclarationList parse_declaration_list();
	public:
		Parser(std::unique_ptr<Lexer> lexer);

		std::unique_ptr<ast::Program> parse();
	};
}