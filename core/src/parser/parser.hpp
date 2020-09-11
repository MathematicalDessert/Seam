#pragma once

#include <memory>

#include "../ir/ast/expression.hpp"
#include "../ir/ast/statement.hpp"
#include "../lexer/lexer.hpp"

namespace seam::core::parser
{
	class parser
	{
		lexer::lexer lexer_;
		ir::ast::statement::block* current_block_ = nullptr;
		
		[[maybe_unused]] lexer::lexeme& expect(lexer::lexeme_type type, bool consume = false);

		[[nodiscard]] std::shared_ptr<ir::ast::variable> get_variable_from_current_block(const std::string& variable_name) const;
		
		std::shared_ptr<types::type> parse_type();
		std::unique_ptr<ir::ast::parameter> parse_parameter();
		ir::ast::statement::parameter_list parse_parameter_list();

		ir::ast::expression::expression_list parse_expression_list();
		std::unique_ptr<ir::ast::expression::call> parse_call_expression(std::string function_name);
		std::unique_ptr<ir::ast::expression::expression> parse_simple_expression(bool use_current_lexeme = false);
		std::pair<std::unique_ptr<ir::ast::expression::expression>, std::optional<lexer::lexeme_type>> parse_expression(std::size_t right_binding_power = 0, bool use_current_lexeme = false);

		std::unique_ptr<ir::ast::statement::ret_stat> parse_return_statement();
		std::unique_ptr<ir::ast::statement::statement> parse_assignment_statement(std::string variable_name);
		
		std::unique_ptr<ir::ast::statement::statement_block> parse_statement_block(const ir::ast::statement::parameter_list* vars = nullptr);
		std::unique_ptr<ir::ast::statement::function_definition> parse_function_definition();
		std::unique_ptr<ir::ast::statement::declaration> parse_declaration();
		std::unique_ptr<ir::ast::statement::declaration_block> parse_declaration_block(bool is_type_scope = false);
	public:
		explicit parser(std::string_view source);

		std::unique_ptr<ir::ast::statement::declaration_block> parse();
	};
}
