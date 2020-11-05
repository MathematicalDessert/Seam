#include "alias_type.hpp"

namespace seam::core::types
{
	alias_type::alias_type(std::shared_ptr<base_type> aliased_type) :
		base_type(internal_type::t_alias), aliased_type(aliased_type)
	{}

	bool alias_type::is_void() const
	{
		return aliased_type->is_void();
	}

	bool alias_type::is_number() const
	{
		return aliased_type->is_number();
	}

	bool alias_type::is_boolean() const
	{
		return aliased_type->is_boolean();
	}

	bool alias_type::is_string() const
	{
		return aliased_type->is_string();
	}

	bool alias_type::is_class() const
	{
		return aliased_type->is_class();
	}

	bool alias_type::is_alias() const
	{
		return aliased_type->is_alias();
	}
	
	std::shared_ptr<base_type> alias_type::get_aliased_type() const
	{
		return aliased_type;
	}

	llvm::Type* alias_type::get_llvm_type(llvm::LLVMContext& context)
	{
		return aliased_type->get_llvm_type(context);
	}
	
	bool alias_type::operator==(const base_type& other_type)
	{
		return &other_type == this || aliased_type.get() == this;
	}
}