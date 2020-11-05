#include "type.hpp"
#include "bool_type.hpp"
#include "number_type.hpp"
#include "string_type.hpp"
#include "void_type.hpp"

namespace seam::core::types
{
	const std::unordered_map<std::string, internal_type> built_in_type_map = {
		{ "void", internal_type::t_void },
		{ "bool", internal_type::t_bool },
		{ "string", internal_type::t_string },
		{ "u8", internal_type::t_u8 },
		{ "i8", internal_type::t_i8 },
		{ "u16", internal_type::t_u16 },
		{ "i16", internal_type::t_i16 },
		{ "u32", internal_type::t_u32 },
		{ "i32", internal_type::t_i32 },
		{ "u64", internal_type::t_u64 },
		{ "i64", internal_type::t_i64 },
		{ "f32", internal_type::t_f32 },
		{ "f64", internal_type::t_f64 },
	};

	constexpr static std::string_view built_in_type_name_array[] = {
		"<undefined>",
		"void",
		"bool",
		"t_string",
		"i8",
		"i16",
		"i32",
		"i64",
		"u8",
		"u16",
		"u32",
		"u64",
		"f32",
		"f64",
		"<alias>",
		"<class>",
		"<count>",
	};
	static_assert(static_cast<int>(internal_type::count) + 1 == std::size(built_in_type_name_array), "internal_type and built_in_type_name_array size mismatch");

	const std::unordered_map<std::string, std::shared_ptr<base_type>> built_in_types = {
		{ "void", std::make_shared<void_type>()->get_base_type() },
		{ "bool", std::make_shared<bool_type>()->get_base_type() },
		{ "string", std::make_shared<string_type>()->get_base_type() },
		{ "u8", std::make_shared<number_type>(internal_type::t_u8, 8)->get_base_type() },
		{ "i8", std::make_shared<number_type>(internal_type::t_i8, 8)->get_base_type() },
		{ "u16", std::make_shared<number_type>(internal_type::t_u16, 16)->get_base_type() },
		{ "i16", std::make_shared<number_type>(internal_type::t_i16, 16)->get_base_type() },
		{ "u32", std::make_shared<number_type>(internal_type::t_u32, 32)->get_base_type() },
		{ "i32", std::make_shared<number_type>(internal_type::t_i32, 32)->get_base_type() },
		{ "u64", std::make_shared<number_type>(internal_type::t_u64, 64)->get_base_type() },
		{ "i64", std::make_shared<number_type>(internal_type::t_i64, 64)->get_base_type() },
		{ "f32", std::make_shared<number_type>(internal_type::t_f32, 32)->get_base_type() },
		{ "f64", std::make_shared<number_type>(internal_type::t_f64, 64)->get_base_type() },
	};
	
	internal_type get_type_from_name(const std::string& type_name)
	{
		return built_in_type_map.at(type_name);
	}

	std::string_view get_typename_from_internal_type(const internal_type type)
	{
		return built_in_type_name_array[static_cast<int>(type)];
	}

	std::shared_ptr<base_type> get_base_type_from_name(const std::string& type_name, const bool safe)
	{
		const auto pair = built_in_types.find(type_name);
		if (pair == built_in_types.cend() && safe)
		{
			return nullptr;
		}
		return pair->second;
	}

	std::shared_ptr<base_type> get_base_type_from_type(internal_type type, const bool safe)
	{
		return get_base_type_from_name(std::string{ built_in_type_name_array[static_cast<int>(type)] });
	}
}
