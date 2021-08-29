#include <ast/ast.h>
#include <ast/print_visitor.h>

namespace seam::ast {
	void PrintVisitor::visit(Program& program) {
		append(L"digraph Program {\n\tProgram");
		for (const auto& decl : program.body) {
			decl->accept(*this);
		}
		append(L"}");
	}

	void PrintVisitor::visit(statement::LetStatement& stat) {
		
	}

	void PrintVisitor::visit(FunctionDeclaration& func) {
		func.body->accept(*this);
	}

	void PrintVisitor::visit(statement::Statement& stat) {
		
	}

	void PrintVisitor::visit(expression::Expression& expr) {
		
	}

	void PrintVisitor::visit(statement::StatementBlock& block) {
		for (const auto& stat : block.statements) {
			stat->accept(*this);
		}
	}

}