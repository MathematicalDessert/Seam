#pragma once

#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>

#include "../ir/ast/statement.hpp"
#include "../module.hpp"

namespace seam::core::code_generation
{
	class code_generator
	{
		llvm::LLVMContext context_;
		std::shared_ptr<llvm::Module> module_;

		llvm::Function* get_or_declare_function(ir::ast::statement::function_definition* func);
		void compile_function(ir::ast::statement::function_definition* function);
	public:
		
		
		explicit code_generator(std::unique_ptr<ir::ast::statement::declaration_block> root);		
	};
}
