#include "alias_type.hpp"

namespace seam::core::types
{
	alias_type::alias_type() :
		base_type(internal_type::t_alias)
	{}

	std::shared_ptr<base_type> alias_type::get_aliased_type() const
	{
		return aliased_type;
	}

	bool alias_type::operator==(const base_type& other_type)
	{
		return &other_type == this || aliased_type.get() == this;
	}
}