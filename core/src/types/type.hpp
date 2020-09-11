#pragma once

#include <string>

namespace seam::core::types
{
	struct type
	{
		std::string name;
		bool is_nullable = false;
		bool is_implicit_type = false;

		explicit type(std::string name, const bool is_nullable = false , const bool is_implicit = false) :
			name(std::move(name)), is_nullable(is_nullable), is_implicit_type(is_implicit)
		{}
	};
}
