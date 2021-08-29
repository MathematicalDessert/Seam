#pragma once

#include <string>
#include "visitor.h"

namespace seam::ast {namespace expression {
		class Expression;
	}

	struct Declaration;
	struct Program;
	struct FunctionDeclaration;
	namespace statement {
		struct Statement;
		struct LetStatement;
		struct StatementBlock;
	}

	class PrintVisitor final : Visitor<Program, statement::StatementBlock, expression::Expression, statement::Statement, FunctionDeclaration, statement::LetStatement> {
		std::wstring output_string_;

		void append(const std::wstring& str) {
			output_string_ += str + L"\n";
		}
	public:
		void visit(Program& program) override;
		void visit(statement::LetStatement& stat) override;
		void visit(FunctionDeclaration& func) override;
		void visit(statement::Statement& stat) override;
		void visit(expression::Expression& expr) override;
		void visit(statement::StatementBlock& block) override;

		[[nodiscard]] std::wstring str() const { return output_string_; };
	};
}