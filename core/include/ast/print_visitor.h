#pragma once

#include <string>
#include "visitor.h"

namespace seam::ast {
	namespace expression {
		struct Expression;
		struct StringLiteral;
		struct NumberLiteral;
		struct BooleanLiteral;
		struct UnaryExpression;
	}

	struct Declaration;
	struct Program;
	struct FunctionDeclaration;
	namespace statement {
		struct Statement;
		struct LetStatement;
		struct StatementBlock;
		struct IfStatement;
	}

	class PrintVisitor final : Visitor <
		Program,
		statement::StatementBlock,
		expression::StringLiteral,
		expression::NumberLiteral,
		expression::BooleanLiteral,
		expression::UnaryExpression,
		FunctionDeclaration,
		statement::LetStatement,
		statement::IfStatement,
		statement::WhileStatement> {
		size_t node_count_ = 0;
		std::wstring output_string_;

		void append(const std::wstring& str) {
			output_string_ += str + L"\n";
		}
	public:
		void visit(Program& program) override;
		void visit(statement::LetStatement& stat) override;
		void visit(FunctionDeclaration& func) override;
		void visit(expression::StringLiteral& expr) override;
		void visit(expression::NumberLiteral& expr) override;
		void visit(expression::BooleanLiteral& expr) override;
		void visit(expression::UnaryExpression& expr) override;
		void visit(statement::StatementBlock& block) override;
		void visit(statement::IfStatement& stat) override;
		void visit(statement::WhileStatement& stat) override;

		[[nodiscard]] std::wstring str() const { return output_string_; };
	};
}