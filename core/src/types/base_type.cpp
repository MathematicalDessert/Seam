#include "base_type.hpp"

namespace seam::core::types
{
	base_type::base_type(std::string name, const internal_type type) :
		name_(name), type_(type)
	{}
	
	base_type::base_type(const internal_type type) :
		type_(type)
	{}

	std::string base_type::get_name() const
	{
		return name_;
	}

	internal_type base_type::get_type() const
	{
		return type_;
	}

	bool base_type::is_void() const
	{
		return type_ == internal_type::t_void;
	}
	
	bool base_type::is_number() const
	{
		switch (type_)
		{
			case internal_type::t_i8:
			case internal_type::t_i16:
			case internal_type::t_i32:
			case internal_type::t_i64:
			case internal_type::t_u8:
			case internal_type::t_u16:
			case internal_type::t_u32:
			case internal_type::t_u64:
			case internal_type::t_f32:
			case internal_type::t_f64:
			{
				return true;
			}
			default:
			{
				return false;
			}
		}
	}

	bool base_type::is_boolean() const
	{
		return type_ == internal_type::t_bool;
	}

	bool base_type::is_string() const
	{
		return type_ == internal_type::t_string;
	}

	bool base_type::is_class() const
	{
		return type_ == internal_type::t_class;
	}

	bool base_type::is_alias() const
	{
		return type_ == internal_type::t_alias;
	}

	std::shared_ptr<base_type> base_type::get_base_type()
	{
		return this->shared_from_this();
	}
}