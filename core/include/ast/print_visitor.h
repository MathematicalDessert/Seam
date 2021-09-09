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
		struct BinaryExpression;
	}

	struct Declaration;
	struct Program;
	struct FunctionDeclaration;
    struct TypeDeclaration;
    struct TypeAliasDeclaration;
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
		statement::WhileStatement,
		TypeDeclaration,
		TypeAliasDeclaration,
		expression::BinaryExpression> {
		size_t node_count_ = 0;
		std::wstring output_string_;

		size_t new_code_count() { return node_count_++; }

		void append(const std::wstring& str) {
			output_string_ += str + L"\n";
		}

		void append_pointer(const std::wstring& lhs, size_t from, size_t to) {
			for (auto i = from; i < to; i++) {
				append(fmt::format(L"{} -> {}", lhs, to));
			}
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
		void visit(expression::BinaryExpression& expr) override;
		void visit(TypeDeclaration& stat) override;
		void visit(TypeAliasDeclaration& stat) override;

		[[nodiscard]] std::wstring str() const { return output_string_; };
	};
}