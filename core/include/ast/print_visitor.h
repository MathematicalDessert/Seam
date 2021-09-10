#pragma once

#include <stack>
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
		struct PostfixExpression;
		struct FunctionExpression;
		struct FunctionCall;
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
		expression::Identifier,
		expression::BinaryExpression,
		expression::PostfixExpression,
		expression::FunctionExpression,
		expression::FunctionCall> {
		size_t node_count_ = 0;
		std::wstring output_string_;

		std::stack<std::wstring> parent_;

		void push_i_parent(const size_t node) {
			push_parent(std::to_wstring(node));
		}

		void push_parent(std::wstring node_name) {
			parent_.push(std::move(node_name));
		}
		std::wstring current_parent() { return parent_.top(); }
		std::wstring pop_parent() {
			auto result = current_parent();
			parent_.pop();
			return result;
		};

		size_t new_code_count() { return node_count_++; }

		void append(const std::wstring& str) {
			output_string_ += str + L"\n";
		}

		void append_pointer(const std::wstring& lhs, size_t from, size_t to) {
			for (auto i = from; i < to; i++) {
				append(fmt::format(L"{} -> {}", lhs, to));
			}
		}

		void draw_parent(size_t node) {
			append(fmt::format(L"{} -> {}", current_parent(), node));
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
		void visit(expression::Identifier& expr) override;
		void visit(expression::BinaryExpression& expr) override;
		void visit(expression::PostfixExpression& expr) override;
		void visit(expression::FunctionExpression& expr) override;
		void visit(expression::FunctionCall& expr) override;
		void visit(TypeDeclaration& stat) override;
		void visit(TypeAliasDeclaration& stat) override;

		[[nodiscard]] std::wstring str() const { return output_string_; };
	};
}