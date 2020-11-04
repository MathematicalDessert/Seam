#pragma once

#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>

#include "../ir/ast/statement.hpp"
#include "../module.hpp"

namespace seam::core::code_generation
{
	class code_generator
	{
		module* module_;

		llvm::LLVMContext context_;
		std::shared_ptr<llvm::Module> llvm_module_;

		std::unordered_map<std::string, llvm::FunctionType*> function_type_map;

		llvm::FunctionType* get_llvm_function_type(ir::ast::statement::function_definition* func);
		llvm::Function* get_or_declare_function(ir::ast::statement::function_definition* func);
		void compile_function(ir::ast::statement::function_definition* function);
	public:
		std::shared_ptr<llvm::Module> generate();

		llvm::Type* get_type(types::internal_type type);

		explicit code_generator(module* module);		
	};
}
