#include "code_generator.hpp"
#include "../ir/ast/visitor.hpp"

namespace seam::core::code_generation
{
	struct function_collector_visitor final : ir::ast::visitor
	{
		std::vector<ir::ast::statement::function_definition*> functions;

		bool visit(ir::ast::statement::function_definition* node) override
		{
			functions.emplace_back(node);
			return true;
		}
	};

	llvm::Function* code_generator::get_or_declare_function(ir::ast::statement::function_definition* func)
	{
		return nullptr;
	}

	void code_generator::compile_function(ir::ast::statement::function_definition* function)
	{
		llvm::Function* llvm_function = nullptr;
	}
	
	code_generator::code_generator(std::unique_ptr<ir::ast::statement::declaration_block> root)
	{
		function_collector_visitor collected_functions;
		root->visit(&collected_functions);

		for (const auto& function : collected_functions.functions)
		{
			
		}
	}
}