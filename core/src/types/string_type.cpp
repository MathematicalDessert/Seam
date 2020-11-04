#include "string_type.hpp"

namespace seam::core::types
{
	string_type::string_type() :
		base_type(internal_type::t_string)
	{}

	llvm::Type* string_type::get_llvm_type(llvm::LLVMContext& context)
	{
		return nullptr; // TODO: String types
	}

	bool string_type::operator==(const base_type& other_type)
	{
		return &other_type == this || type_ == internal_type::t_string;
	}
}