#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>

#include "code_generator.hpp"
#include "../ir/ast/visitor.hpp"
#include "../types/number_type.hpp"
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
		std::unordered_map<std::string, llvm::Value*> params;
		std::unordered_map<ir::ast::expression::variable*, llvm::Value*> variables;
		ir::ast::statement::block* current_block = nullptr;

		llvm::Value* value = nullptr;

		explicit code_generator_visitor(llvm::IRBuilder<>& builder, code_generator& gen, std::unordered_map<std::string, llvm::Value*> vars) :
			builder(builder), generator(gen), params(std::move(vars))
		{}

		bool visit(ir::ast::expression::bool_literal* node) override
		{
			value = llvm::ConstantInt::get(builder.getContext(), llvm::APInt(1, node->value));
			return false;
		}

		bool visit(ir::ast::expression::number_literal* node) override
		{
			const auto type = static_cast<types::number_type*>(node->type.get());

			if (type->is_float())
			{
				const auto numerical_value = std::get<double>(node->value);
				switch (type->get_type())
				{
					case types::internal_type::t_f32: value = llvm::ConstantFP::get(builder.getContext(), llvm::APFloat(static_cast<float>(numerical_value))); break;
					case types::internal_type::t_f64: value = llvm::ConstantFP::get(builder.getContext(), llvm::APFloat(numerical_value)); break;
					default:
					{
						throw utils::compiler_exception{ node->range.start, "internal compiler error: unknown floating point type" };
					}
				}
			}
			else if (type->is_integer())
			{
				value = llvm::ConstantInt::get(builder.getContext(), llvm::APInt(type->get_size(), std::get<std::uint64_t>(node->value)));
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

		bool visit(ir::ast::statement::if_stat* node) override
		{
			node->condition->visit(this);
			auto condition_value = value;

			auto start_block = builder.GetInsertBlock();

			auto main_body_block = llvm::BasicBlock::Create(builder.getContext(), "body",
				start_block->getParent());

			builder.SetInsertPoint(main_body_block);
			node->body->visit(this);

			if (node->else_body)
			{
				auto else_body_block = llvm::BasicBlock::Create(builder.getContext(), "else_body",
					start_block->getParent());

				builder.SetInsertPoint(else_body_block);
				node->else_body->visit(this);

				auto end_block = llvm::BasicBlock::Create(builder.getContext(), "exit",
					start_block->getParent());

				builder.SetInsertPoint(start_block);
				builder.CreateCondBr(condition_value, main_body_block, else_body_block);

				if (!main_body_block->getTerminator())
				{
					builder.SetInsertPoint(main_body_block);
					builder.CreateBr(end_block);
				}

				if (!else_body_block->getTerminator())
				{
					builder.SetInsertPoint(else_body_block);
					builder.CreateBr(end_block);
				}

				builder.SetInsertPoint(end_block);
			}
			else
			{
				auto end_block = llvm::BasicBlock::Create(builder.getContext(), "exit",
					start_block->getParent());

				if (!start_block->getTerminator())
				{
					builder.SetInsertPoint(start_block);
					builder.CreateCondBr(condition_value, main_body_block, end_block);
				}

				if (!main_body_block->getTerminator())
				{
					builder.SetInsertPoint(main_body_block);
					builder.CreateBr(end_block);
				}

				builder.SetInsertPoint(end_block);
			}
			return false;
		}
		
		bool visit(ir::ast::expression::variable* node) override
		{
			const auto& p_it = params.find(node->name);
			if (p_it != params.cend())
			{
				value = p_it->second;
			}
			else
			{
				const auto& it = variables.find(node);
				if (it != variables.cend())
				{
					value = it->second;
				}
				else
				{
					auto var = current_block->get_variable(node->name);
					value = builder.CreateAlloca(var->type->get_llvm_type(builder.getContext()), nullptr);
				}
			}
			return false;
		}

		bool visit(ir::ast::expression::binary* node) override
		{
			node->left->visit(this);
			auto lhs_value = value;

			node->right->visit(this);
			auto rhs_value = value;

			// TODO: correct?
			bool unsigned_operation = true;//resolved_left->is_unsigned && resolved_right->is_unsigned;

			// If either left or right is floating point, use a floating point operation.
		   /* bool float_operation = std::visit(
				[&float_operation](auto&& left_value, auto&& right_value) -> bool
				{
					using left_value_t = std::decay_t<decltype(left_value)>;
					using right_value_t = std::decay_t<decltype(right_value)>;
					return (std::is_same_v<left_value_t, float> || std::is_same_v<left_value_t, double>)
						|| (std::is_same_v<right_value_t, float> || std::is_same_v<right_value_t, double>);
				}, resolved_left->value, resolved_right->value);*/

			const auto float_operation = false;

			if (lhs_value->getType()->isPointerTy())
			{
				lhs_value = builder.CreateLoad(lhs_value);
			}

			if (rhs_value->getType()->isPointerTy())
			{
				rhs_value = builder.CreateLoad(rhs_value);
			}
			
			switch (node->operation)
			{
			case lexer::lexeme_type::symbol_add:
			{
				if (float_operation)
				{
					value = builder.CreateFAdd(lhs_value, rhs_value, "faddtmp");
				}
				else
				{
					value = builder.CreateAdd(lhs_value, rhs_value, "addtmp");
				}
				break;
			}
			case lexer::lexeme_type::symbol_subtract:
			{
				if (float_operation)
				{
					value = builder.CreateFSub(lhs_value, rhs_value, "fsubtmp");
				}
				else
				{
					value = builder.CreateSub(lhs_value, rhs_value, "subtmp");
				}
				break;
			}
			case lexer::lexeme_type::symbol_multiply:
			{
				if (float_operation)
				{
					value = builder.CreateFMul(lhs_value, rhs_value, "fmultmp");
				}
				else
				{
					value = builder.CreateMul(lhs_value, rhs_value, "multmp");
				}
				break;
			}
			case lexer::lexeme_type::symbol_divide:
			{
				if (float_operation)
				{
					value = builder.CreateFDiv(lhs_value, rhs_value, "fdivtmp");
				}
				else if (unsigned_operation)
				{
					value = builder.CreateUDiv(lhs_value, rhs_value, "udivtmp");
				}
				else
				{
					value = builder.CreateSDiv(lhs_value, rhs_value, "sdivtmp");
				}
				break;
			}
			case lexer::lexeme_type::symbol_eq:
			{
				if (float_operation)
				{
					value = builder.CreateFCmpOEQ(lhs_value, rhs_value, "feqtmp");
				}
				else
				{
					value = builder.CreateICmpEQ(lhs_value, rhs_value, "eqtmp");
				}
				break;
			}
			case lexer::lexeme_type::symbol_neq:
			{
				if (float_operation)
				{
					value = builder.CreateFCmpONE(lhs_value, rhs_value, "fnetmp");
				}
				else
				{
					value = builder.CreateICmpNE(lhs_value, rhs_value, "netmp");
				}
				break;
			}
			case lexer::lexeme_type::symbol_lt:
			{
				if (float_operation)
				{
					value = builder.CreateFCmpOLT(lhs_value, rhs_value, "folttmp");
				}
				else if (unsigned_operation)
				{
					value = builder.CreateICmpULT(lhs_value, rhs_value, "ulttmp");
				}
				else
				{
					value = builder.CreateICmpSLT(lhs_value, rhs_value, "slttmp");
				}
				break;
			}
			case lexer::lexeme_type::symbol_gt:
			{
				if (float_operation)
				{
					value = builder.CreateFCmpOGT(lhs_value, rhs_value, "fogttmp");
				}
				else if (unsigned_operation)
				{					
					value = builder.CreateICmpUGT(lhs_value, rhs_value, "ugttmp");
				}
				else
				{
					value = builder.CreateICmpSGT(lhs_value, rhs_value, "sgttmp");
				}
				break;
			}
			default:
			{
				throw utils::compiler_exception{ node->range.start, "internal compiler error: invalid binary operation" };
			}
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

	llvm::FunctionType* code_generator::get_llvm_function_type(ir::ast::statement::function_definition* function)
	{
		if (const auto it = function_type_map.find(function->mangled_name); it != function_type_map.cend())
		{
			return it->second;
		}

		std::vector<llvm::Type*> param_types;
		for (const auto& param : function->parameters)
		{
			const auto param_type = param->type->get_llvm_type(context_);
			if (!llvm::FunctionType::isValidArgumentType(param_type))
			{
				throw utils::compiler_exception(function->range.start, "internal compiler error: invalid parameter type");
			}
			param_types.push_back(param_type);
		}		

		auto type = function->return_type->get_llvm_type(context_);

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

		auto args = llvm_function->arg_begin();
		for (const auto& param : function->parameters)
		{
			args++->setName(param->name);
		}
		
		return llvm_function;
	}

	void code_generator::compile_function(ir::ast::statement::function_definition* function)
	{
		llvm::Function* llvm_function = get_or_declare_function(function);

		llvm::BasicBlock* basic_block = llvm::BasicBlock::Create(context_, "entry",
			llvm_function);
		llvm::IRBuilder<> builder(basic_block);

		std::unordered_map<std::string, llvm::Value*> params;

		auto args = llvm_function->arg_begin();
		for (const auto& param : function->parameters)
		{
			params.emplace(param->name, args++);
		}
		
		code_generator_visitor code_gen{ builder, *this, params };
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