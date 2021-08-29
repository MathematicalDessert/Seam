#pragma once

#include <utility>
#include <vector>
#include <memory>

#include <exception.h>

namespace seam::ast {
	class PrintVisitor;

	template <typename... Visitors>
	struct Node;

	template <typename Visitor>
	struct Node<Visitor> {
		virtual void accept(Visitor& visitor) = 0;
	};

	template<typename Derived, typename Visitor>
	struct Node<Derived, Visitor> : virtual Node<Visitor> {
		void accept(Visitor& visitor) override {
			visitor.visit(dynamic_cast<Derived&>(*this));
		}
	};

	template<typename Derived, typename Visitor, typename... Visitors>
	struct Node<Derived, Visitor, Visitors...> : virtual Node<Derived, Visitors...>, virtual Node<Visitor>
	{
		using Node<Derived, Visitors...>::accept;

		void accept(Visitor& visitor) override {
			visitor.visit(dynamic_cast<Derived&>(*this));
		}
	};

	struct Parameter {
		std::wstring name;
		std::wstring type;
	};
	using ParameterList = std::vector<Parameter>;

	namespace expression {
		struct Expression : virtual Node<PrintVisitor> { };

		struct Literal : Expression, virtual Node<PrintVisitor> {
			std::wstring value;

		protected:
			explicit Literal(std::wstring value)
				: value(std::move(value)) {}
		};

		struct StringLiteral : Literal, Node<StringLiteral, PrintVisitor> {
			explicit StringLiteral(std::wstring value)
				: Literal(std::move(value)) {}
		};

		struct NumberLiteral : Literal, Node<NumberLiteral, PrintVisitor> {
			explicit NumberLiteral(std::wstring value)
				: Literal(std::move(value)) {}
		};
	}
	
	namespace statement {
		struct Statement : virtual Node<PrintVisitor> {

		};
		using StatementList = std::vector<std::unique_ptr<Statement>>;

		struct LetStatement : Statement, Node<LetStatement, PrintVisitor> {
			std::wstring name;
			std::wstring type;
			std::unique_ptr<expression::Expression> expr;

			LetStatement(std::wstring name, std::wstring type, std::unique_ptr<expression::Expression> expr)
				: name(std::move(name)), expr(std::move(expr)) {}
		};

		struct StatementBlock : Statement, Node<StatementBlock, PrintVisitor> {
			StatementList statements;

			StatementBlock(
				StatementList list
			) : statements(std::move(list)) {}
		};
	}

	struct Declaration : virtual Node<PrintVisitor> { };
	using DeclarationList = std::vector<std::unique_ptr<Declaration>>;

	struct FunctionDeclaration : Declaration, Node<FunctionDeclaration, PrintVisitor> {
		std::wstring name;
		ParameterList params;
		std::wstring return_type;
		std::unique_ptr<statement::StatementBlock> body;

		FunctionDeclaration(
			std::wstring name,
			ParameterList params,
			std::wstring return_type,
			std::unique_ptr<statement::StatementBlock> block)
				: name(std::move(name)), params(std::move(params)), return_type(std::move(return_type)), body(std::move(block)){}
	};

	struct Program : Node<Program, PrintVisitor> {
		DeclarationList body;

		Program(DeclarationList body)
			: body(std::move(body)) {}
	};
}
