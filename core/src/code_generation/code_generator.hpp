#pragma once

#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>

#include "../ir/ast/statement.hpp"

namespace seam::core::code_generation
{
	class code_generator
	{
		llvm::LLVMContext context_;
		std::shared_ptr<llvm::Module> module_;
	public:
		explicit code_generator(std::unique_ptr<ir::ast::statement::declaration_block> root);		
	};
}
