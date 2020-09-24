#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>

#include "code_generator.hpp"
#include "../ir/ast/visitor.hpp"
#include "../utils/exception.hpp"

namespace seam::core::code_generation
{
	struct function_collector_visitor final : ir::ast::visitor
	{
		std::vector<ir::ast::statement::function_definition*> functions;

		bool visit(ir::ast::statement::function_definition* node) override
		{
			functions.emplace_back(node);
			return false;
		}
	};

	struct code_generator_visitor final : ir::ast::visitor
	{
		llvm::IRBuilder<>& builder;
		code_generator& generator;
		std::unordered_map<ir::ast::expression::variable*, llvm::Value*> variables;
		ir::ast::statement::block* current_block = nullptr;

		llvm::Value* value = nullptr;

		explicit code_generator_visitor(llvm::IRBuilder<>& builder, code_generator& gen) :
			builder(builder), generator(gen)
		{}

		bool visit(ir::ast::expression::bool_literal* node) override
		{
			value = llvm::ConstantInt::get(builder.getContext(), llvm::APInt(1, node->value));
			return false;
		}

		bool visit(ir::ast::expression::number_literal* node) override
		{
			if (std::holds_alternative<double>(node->value))
			{
				auto numerical_value = std::get<double>(node->value);
				switch (node->type->internal_type)
				{
					case types::built_in_type::f32: value = llvm::ConstantFP::get(builder.getContext(), llvm::APFloat(static_cast<float>(numerical_value))); break;
					case types::built_in_type::f64: value = llvm::ConstantFP::get(builder.getContext(), llvm::APFloat(numerical_value)); break;
					default:
					{
						throw utils::compiler_exception{ node->range.start, "internal compiler error: unknown floating point type" };
					}
				}
			}
			else
			{
				auto numerical_value = std::get<std::uint64_t>(node->value);
				value = llvm::ConstantInt::get(builder.getContext(), llvm::APInt(get_number_bytes_size(node->type->internal_type) * 8, numerical_value));
			}
		}

		bool visit(ir::ast::statement::ret_stat* node) override
		{
			if (node->value)
			{
				node->value->visit(this); // generate return
				if (llvm::isa<llvm::AllocaInst>(value))
				{
					value = builder.CreateLoad(value);
				}
				builder.CreateRet(value);
			}
			else
			{
				builder.CreateRetVoid();
			}
			return false;
		}

		bool visit(ir::ast::expression::variable* node) override
		{
			const auto& it = variables.find(node);
			if (it != variables.cend())
			{
				value = it->second;
			}
			else
			{
				auto var = current_block->get_variable(node->name);
				value = builder.CreateAlloca(generator.get_type(var->type->internal_type), nullptr);
			}
			return false;
		}

		bool visit(ir::ast::statement::statement_block* node) override
		{
			current_block = node;
			return true;
		}

		bool visit(ir::ast::statement::declaration_block* node) override
		{
			current_block = node;
			return true;
		}
	};

	llvm::Type* code_generator::get_type(types::built_in_type type)
	{
		switch (type)
		{
		case types::built_in_type::void_: return llvm::Type::getVoidTy(context_);
		case types::built_in_type::bool_: return llvm::Type::getInt1Ty(context_);
		case types::built_in_type::u8: case types::built_in_type::i8: return llvm::Type::getInt8Ty(context_);
		case types::built_in_type::u16: case types::built_in_type::i16: return llvm::Type::getInt16Ty(context_);
		case types::built_in_type::u32: case types::built_in_type::i32: return llvm::Type::getInt32Ty(context_);
		case types::built_in_type::u64: case types::built_in_type::i64: return llvm::Type::getInt64Ty(context_);
		}

		return nullptr;
	}

	llvm::FunctionType* code_generator::get_llvm_function_type(ir::ast::statement::function_definition* function)
	{
		if (const auto it = function_type_map.find(function->mangled_name); it != function_type_map.cend())
		{
			return it->second;
		}

		std::vector<llvm::Type*> param_types;
		for (const auto& param : function->parameters)
		{
			const auto param_type = get_type(param->type->internal_type);
			if (!llvm::FunctionType::isValidArgumentType(param_type))
			{
				throw utils::compiler_exception(function->range.start, "internal compiler error: invalid parameter type");
			}
			param_types.push_back(param_type);
		}


		auto type = get_type(function->return_type->internal_type == types::built_in_type::undefined ? function->block->return_type->internal_type : function->return_type->internal_type);

		const auto function_type = llvm::FunctionType::get(type, llvm::makeArrayRef(param_types), false);
		function_type_map.emplace(function->mangled_name, function_type);

		return function_type;
	}

	llvm::Function* code_generator::get_or_declare_function(ir::ast::statement::function_definition* function)
	{
		auto llvm_function = llvm_module_->getFunction(function->mangled_name);

		if (!llvm_function)
		{
			llvm_function = llvm::Function::Create(get_llvm_function_type(function), llvm::GlobalValue::InternalLinkage, function->mangled_name, *llvm_module_);
		}

		return llvm_function;
	}

	void code_generator::compile_function(ir::ast::statement::function_definition* function)
	{
		llvm::Function* llvm_function = get_or_declare_function(function);

		llvm::BasicBlock* basic_block = llvm::BasicBlock::Create(context_, "entry",
			llvm_function);
		llvm::IRBuilder<> builder(basic_block);

		code_generator_visitor code_gen{ builder, *this };
		function->block->visit(&code_gen);


		std::string error;
		llvm::raw_string_ostream error_stream{ error };
		if (llvm::verifyFunction(*llvm_function, &error_stream))
		{
			// TODO: Throw exception here!
		}
	}
	
	std::shared_ptr<llvm::Module> code_generator::generate()
	{
		function_collector_visitor collected_functions;
		module_->get_root()->visit(&collected_functions);

		for (const auto& function : collected_functions.functions)
		{
			compile_function(function);
		}

		return llvm_module_;
	}

	code_generator::code_generator(module* module) :
		module_(module), llvm_module_(std::make_shared<llvm::Module>(module->get_name(), context_))
	{}
}