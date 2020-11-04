#include "void_type.hpp"

namespace seam::core::types
{
	void_type::void_type() :
		base_type(internal_type::t_void)
	{}
	
	llvm::Type* void_type::get_llvm_type(llvm::LLVMContext& context)
	{
		return llvm::Type::getVoidTy(context);
	}

	bool void_type::operator==(const base_type& other_type)
	{
		// void types cannot be equal.
		return false;
	}
}