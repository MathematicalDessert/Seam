#pragma once

#include <optional>
#include <string>

namespace seam::core::types
{
	enum class built_in_type
	{
		void_,
		bool_,
		i8,
		u8,
		i16,
		u16,
		i32,
		u32,
		i64,
		u64,
		f32,
		f64,
	};

	const std::unordered_map<std::string, built_in_type> built_in_type_map = {
		{ "void", built_in_type::void_ },
		{ "bool", built_in_type::bool_ },
		{ "u8", built_in_type::u8 },
		{ "i8", built_in_type::i8 },
		{ "u16", built_in_type::u16 },
		{ "i16", built_in_type::i16 },
		{ "u32", built_in_type::u32 },
		{ "i32", built_in_type::i32 },
		{ "u64", built_in_type::u64 },
		{ "i64", built_in_type::i64 },
		{ "f32", built_in_type::f32 },
		{ "f64", built_in_type::f64 },
	};

	inline std::optional<built_in_type> string_to_type(const std::string& type)
	{
		if (const auto iterator = built_in_type_map.find(type); iterator != built_in_type_map.cend())
		{
			return iterator->second;
		}
		return {};
	}
	
	struct type
	{
		std::string name;
		built_in_type internal_type;
		bool is_nullable = false;
		bool is_implicit_type = false;

		explicit type(std::string name, const bool is_nullable = false , const bool is_implicit = false) :
			name(std::move(name)), is_nullable(is_nullable), is_implicit_type(is_implicit)
		{}
	};
}
