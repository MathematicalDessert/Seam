#include "bool_type.hpp"

namespace seam::core::types
{
	bool_type::bool_type() :
		base_type(internal_type::t_bool)
	{}

	llvm::Type* bool_type::get_llvm_type(llvm::LLVMContext& context)
	{
		return llvm::Type::getVoidTy(context);
	}

	bool bool_type::operator==(const base_type& other_type)
	{
		return this == &other_type || other_type.get_type() == internal_type::t_bool;
	}
}