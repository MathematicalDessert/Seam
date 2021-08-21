#pragma once

#include <vector>
#include <memory>

#include <exception.h>

namespace seam::ast {
	class Visitor;

	template <typename... Visitors>
	class Node;

	template <typename Visitor>
	class Node<Visitor> {
		virtual void accept(Visitor& visitor) {
			throw SeamException(L"");
		};
	};

	template <typename Derived, typename Visitor>
	class Node<Derived, Visitor> : Node<Visitor> {
		void accept(Visitor& visitor) override {
			visitor.visit(dynamic_cast<Derived&>(*this));
		}
	};

	template<typename Derived, typename V, typename... Visitors>
	class Node<Derived, V, Visitors...> : public Node<Derived, Visitors...>, Node<V>
	{
		using Node<Derived, Visitors...>::accept;

		void accept(V& visitor) override {
			visitor.visit(static_cast<Derived&>(*this));
		}
	};
	
	namespace statement {
		class Statement {
			
		};

		using StatementList = std::vector<std::unique_ptr<Statement>>;
	}

	class Declaration {
		
	};
	using DeclarationList = std::vector<std::unique_ptr<Declaration>>;

	class FunctionDeclaration : public Declaration {
		std::wstring name;
		statement::StatementList body;
	public:
		FunctionDeclaration(std::wstring name, statement::StatementList list);
	};

	class Program {
		DeclarationList body;
	public:
		Program(DeclarationList body);
	};
}
