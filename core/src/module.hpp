#pragma once

#include <memory>
#include <string>
#include <vector>
#include <llvm/IR/Module.h>

#include "parser/parser.hpp"

namespace seam::core
{
	class module
	{
		std::string name_;
		std::string_view source_;
		std::vector<std::string> linking_modules_;
		std::unique_ptr<ir::ast::statement::declaration_block> root_;

		friend class lexer::lexer;
	public:
		std::shared_ptr<llvm::Module> compile();

		explicit module(std::string name, const std::string_view source) :
			name_(std::move(name)), source_(source)
		{}

		std::string get_name() { return name_; }
		void set_name(std::string new_name) { name_ = new_name; }

		ir::ast::statement::declaration_block* get_root() { return root_.get(); }
		void set_root(std::unique_ptr<ir::ast::statement::declaration_block> new_root) { root_ = std::move(new_root); }
	};
}
