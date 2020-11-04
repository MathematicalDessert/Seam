#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include "base_type.hpp""

namespace seam::core::types
{
	internal_type get_type_from_name(const std::string& type_name);
	std::string_view get_typename_from_internal_type(internal_type type);
	std::shared_ptr<base_type> get_base_type_from_name(const std::string& type_name, bool safe = true);
	
	// nullable is a property of the variable, and NOT the type.
	// could also check constraints _on_ type classes themselves.
}
