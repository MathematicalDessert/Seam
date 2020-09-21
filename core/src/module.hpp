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
	public:
		std::shared_ptr<llvm::Module> compile();

		explicit module(std::string name, const std::string_view source) :
			name_(std::move(name)), source_(source)
		{}

		friend class parser::parser;
		friend class lexer::lexer;
	};
}
