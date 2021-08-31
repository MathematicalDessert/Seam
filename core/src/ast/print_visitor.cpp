#include <ast/ast.h>
#include <ast/print_visitor.h>
#include <fmt/format.h>

namespace seam::ast {
	void PrintVisitor::visit(Program& program) {
		append(L"digraph Program {\n\tProgram");
		for (const auto& decl : program.body) {
			const auto next_decl = node_count_ + 1;
			decl->accept(*this);
			append(fmt::format(L"Program -> {}", next_decl));
		}
		append(L"}");
	}

	void PrintVisitor::visit(statement::LetStatement& stat) {
		auto this_node = ++node_count_;
		auto type = !stat.type.empty() ? stat.type : L"auto";
		append(fmt::format(LR"({} [shape=record label="{{LetStatement | {{ {} | {} }} }}"])", node_count_, type.c_str(), stat.name.c_str()));
		stat.expr->accept(*this);
		append(fmt::format(L"{} -> {}", node_count_ - 1, node_count_));
	}

	void PrintVisitor::visit(FunctionDeclaration& func) {
		const auto current_node = ++node_count_;
		auto type = !func.return_type.empty() ? func.return_type : L"auto";
		append(fmt::format(LR"({} [shape=record label="{{Function Declaration | {{ {} | {} }} }}"])", node_count_, type.c_str(), func.name.c_str()));
		func.body->accept(*this);
		append(fmt::format(L"{} -> {}", current_node, current_node + 1));
	}

	void PrintVisitor::visit(expression::StringLiteral& expr) {
		append(fmt::format(L"{} [shape=record label=\"{{StringLiteral | {}}}\"]", ++node_count_, expr.value));
	}

	void PrintVisitor::visit(expression::NumberLiteral& expr) {
		append(fmt::format(L"{} [shape=record label=\"{{NumberLiteral | {}}}\"]", ++node_count_, expr.value));
	}

	void PrintVisitor::visit(expression::BooleanLiteral& expr) {
		append(fmt::format(L"{} [shape=record label=\"{{BooleanLiteral | {}}}\"]", ++node_count_, expr.value));
	}

	void PrintVisitor::visit(statement::StatementBlock& block) {
		const auto current_node = ++node_count_;
		append(fmt::format(L"{} [shape=record label=\"{{StatementBlock}}\"]", current_node));

		for (const auto& stat : block.statements) {
			const auto next_decl = node_count_ + 1;
			stat->accept(*this);
			append(fmt::format(L"{} -> {}", current_node, next_decl));
		}
	}

	void PrintVisitor::visit(statement::IfStatement& stat) {
		
	}

	void PrintVisitor::visit(statement::WhileStatement& stat) {

	}
}