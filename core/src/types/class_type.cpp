#include "class_type.hpp"

#include <sstream>

namespace seam::core::types
{
	class_type::class_type(std::string name) :
		base_type(name, internal_type::t_class)
	{}

	void class_type::add_member(const std::string& member_name, const std::shared_ptr<base_type>& member_type)
	{
		if (members.find(member_name) != members.cend())
		{
			std::stringstream error_message;
			error_message << "'"
				<< member_name
				<< "' is already a member of "
				<< name_;
			throw std::exception(error_message.str().c_str());
		}
		members.emplace(member_name, member_type);
	}

	bool class_type::member_exists(const std::string& member_name)
	{
		return members.find(member_name) != members.cend();
	}

	base_type* class_type::get_member_type(const std::string& member_name)
	{
		if (const auto member_type = members.find(member_name); member_type != members.cend())
		{
			return member_type->second.get();
		}
		return nullptr;
	}

	bool class_type::operator==(const base_type& other_type)
	{
		return type_ == other_type.get_type() && &other_type == this;
	}
}