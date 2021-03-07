#pragma once

#include "ASTNode.h"
#include "statement/Statement.h"

#include <vector>
#include <memory>
#include <string_view>

namespace seam::ast {
	class Declaration : ASTNode {};

	class DeclarationBlock : ASTNode {
	    std::vector<std::unique_ptr<Declaration>> declarations_;
	public:
		void add_declaration(std::unique_ptr<Declaration> declaration);
	};

	class FunctionDeclaration final : public Declaration {
		std::string_view name_;
		statement::StatementList body_;
	public:
		explicit FunctionDeclaration(std::string name)
			: name_(name) {
			
		}

		std::string_view get_name() const { return name_; }
		statement::StatementList& get_body() { return body_; }
	};

	class TypeDeclaration : public Declaration {
		std::string_view name_;
	public:
		explicit TypeDeclaration(std::string name)
			: name_(name) {
			
		}

		std::string_view get_name() const { return name_; }
	};

	class TypeAliasDeclaration : public TypeDeclaration {
	public:
		explicit TypeAliasDeclaration(std::string name)
			: TypeDeclaration(name) {
			
		}
	};
} // namespace seam::ast