#include <ast/ast.h>
#include <ast/print_visitor.h>
#include <fmt/format.h>
#ifndef _WIN32
#include <fmt/xchar.h>
#endif

// TODO: refactor this!

namespace seam::ast {
	void PrintVisitor::visit(Program& program) {
		append(L"digraph Program {\nProgram");

		push_parent(L"Program");
		for (const auto& decl : program.body) {
			decl->accept(*this);
		}
		pop_parent();

		append(L"}");
	}

	void PrintVisitor::visit(statement::LetStatement& stat) {
		const auto type = !stat.type.empty() ? stat.type : L"auto";

		if (type == L"<DISCARD>") {
			stat.expr->accept(*this);
			return;
		}

		const auto this_node = ++node_count_;

		append(fmt::format(LR"({} [shape=record label="{{LetStatement | {{ {} | {} }} }}"])", this_node, type.c_str(), stat.name.c_str()));

		push_i_parent(this_node);
		stat.expr->accept(*this);
		pop_parent();

		// draw parent
		draw_parent(this_node);
	}

	void PrintVisitor::visit(FunctionDeclaration& func) {
		const auto this_node = ++node_count_;
		auto type = !func.return_type.empty() ? func.return_type : L"auto";
		append(fmt::format(LR"({} [shape=record label="{{Function Declaration | {{ {} | {} }} }}"])", this_node, type.c_str(), func.name.c_str()));

		push_i_parent(this_node);
		func.body->accept(*this);
		pop_parent();

		// draw parent
		draw_parent(this_node);
	}

	void PrintVisitor::visit(expression::StringLiteral& expr) {
		const auto this_node = ++node_count_;
		append(fmt::format(L"{} [shape=record label=\"{{StringLiteral | {}}}\"]", this_node, expr.value));

		// draw parent
		draw_parent(this_node);
	}

	void PrintVisitor::visit(expression::NumberLiteral& expr) {
		const auto this_node = ++node_count_;
		append(fmt::format(L"{} [shape=record label=\"{{NumberLiteral | {}}}\"]", this_node, expr.value));

		// draw parent
		draw_parent(this_node);
	}

	void PrintVisitor::visit(expression::BooleanLiteral& expr) {
		const auto this_node = ++node_count_;
		append(fmt::format(L"{} [shape=record label=\"{{BooleanLiteral | {}}}\"]", this_node, expr.value));

		// draw parent
		draw_parent(this_node);
	}

	void PrintVisitor::visit(expression::UnaryExpression& expr) {
		const auto this_node = ++node_count_;
		append(fmt::format(LR"({} [label="{}"])", this_node, token_type_to_name(expr.op)));

		push_i_parent(this_node);
		expr.expr->accept(*this);
		pop_parent();

		// draw parent
		draw_parent(this_node);
	}

	void PrintVisitor::visit(statement::StatementBlock& block) {
		//const auto this_node = ++node_count_;
		//append(fmt::format(L"{} [shape=record label=\"{{StatementBlock}}\"]", this_node));

		for (const auto& stat : block.statements) {
			//push_i_parent(this_node);
			stat->accept(*this);
			//pop_parent();
		}

		//draw_parent(this_node);
	}

	void PrintVisitor::visit(statement::IfStatement& stat) {
		const auto this_node = ++node_count_;
		append(fmt::format(L"{} [shape=record label=\"{{IfStatement}}\"]", this_node));

		push_i_parent(this_node);
		stat.cond->accept(*this);
		stat.body->accept(*this);
		if (stat.else_body) {
			stat.else_body->accept(*this);
		}
		pop_parent();

		draw_parent(this_node);
	}

	void PrintVisitor::visit(statement::WhileStatement& stat) {
		const auto this_node = ++node_count_;
		append(fmt::format(L"{} [shape=record label=\"{{WhileStatement}}\"]", this_node));

		push_i_parent(this_node);
		stat.cond->accept(*this);
		stat.body->accept(*this);
		pop_parent();

		draw_parent(this_node);
	}

	void PrintVisitor::visit(expression::Identifier& expr) {
		auto this_node = ++node_count_;
		append(fmt::format(L"{} [label=\"{}\"]", this_node, expr.identifier));
		draw_parent(this_node);
	}

	void PrintVisitor::visit(expression::BinaryExpression& expr) {
		const auto this_node = ++node_count_;

		append(fmt::format(LR"({} [label="{}"])", this_node, token_type_to_name(expr.op)));

		push_i_parent(this_node);
		expr.lhs->accept(*this);
		expr.rhs->accept(*this);
		pop_parent();

		// draw parent
		draw_parent(this_node);
	}

	void PrintVisitor::visit(expression::PostfixExpression& expr) {
		
	}

	void PrintVisitor::visit(expression::FunctionExpression& expr) {
		
	}

	void PrintVisitor::visit(expression::FunctionCall& expr) {
		const auto this_node = ++node_count_;

		append(fmt::format(L"{} [shape=record label=\"{{FunctionCall}}\"]", this_node));

		push_i_parent(this_node);
		expr.function->accept(*this);

		for (const auto& arg : expr.args) {
			arg->accept(*this);
		}
		pop_parent();

		// draw parent
		draw_parent(this_node);
	}
	
    void PrintVisitor::visit(TypeDeclaration& stat) {

    }

    void PrintVisitor::visit(TypeAliasDeclaration& stat) {

    }
    //TODO: BinaryExpr visitor
}