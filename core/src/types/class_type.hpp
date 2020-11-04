#pragma once

#include "base_type.hpp"

namespace seam::core::types
{
	class class_type : public base_type
	{
		std::unordered_map<std::string, std::shared_ptr<base_type>> members;
	public:
		explicit class_type();
		
		void add_member(const std::string& member_name, const std::shared_ptr<base_type>& member_type);
		bool member_exists(const std::string& member_name);
		base_type* get_member_type(const std::string& member_name);

		bool operator==(const base_type& other_type) override;
	};
}