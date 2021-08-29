#pragma once

#include <ast/ast.h>
#include "lexer.h"

namespace seam {
	class Parser {
		std::unique_ptr<Lexer> lexer_;
		
		template <SymbolType T>
		void expect(const bool consume = true) const {

			if (const SymbolType type = lexer_->peek(); T != type) {
				const auto token = lexer_->next();

				constexpr auto symb_name = symbol_type_to_name_cexpr<T>();
				throw generate_exception<ParserException>(
					token->position,
					L"expected {}, got {}",
					symb_name,
					symbol_type_to_name(type)
					);
			}

			if (consume) {
				lexer_->next();
			}
		}

		template <SymbolType TT, typename T>
		[[nodiscard]] auto consume_token() const {
			expect<TT>(false);

			const auto token = lexer_->next();
			if constexpr (std::is_same_v<T, std::wstring>) {
				return token->lexeme;
			}

			if constexpr (std::is_same_v<T, int>) {
				return 1; // TODO: parse numbers
			}
		}

		std::wstring try_parse_type();
		ast::ParameterList parse_parameter_list();

		std::unique_ptr<ast::expression::Expression> parse_expression();

		std::unique_ptr<ast::statement::Statement> parse_statement();

		std::unique_ptr<ast::statement::LetStatement> parse_let_statement();
		std::unique_ptr<ast::statement::StatementBlock> parse_statement_block();

		std::unique_ptr<ast::FunctionDeclaration> parse_function_declaration();
		ast::DeclarationList parse_declaration_list();
	public:
		Parser(std::unique_ptr<Lexer> lexer);

		std::unique_ptr<ast::Program> parse();
	};
}