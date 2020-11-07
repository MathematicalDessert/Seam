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

	template<typename RangeType, typename ValueType>
	bool is_in_numerical_limits(ValueType value)
	{
		if (!std::numeric_limits<RangeType>::is_integer)
		{
			return  (value > 0 ? value : -value) <= std::numeric_limits<RangeType>::max();
		}

		if (std::numeric_limits<RangeType>::is_signed == std::numeric_limits<ValueType>::is_signed)
		{
			return value >= std::numeric_limits<RangeType>::min() &&
				value <= std::numeric_limits<RangeType>::max();
		}

		if (std::numeric_limits<RangeType>::is_signed)
		{
			return value <= std::numeric_limits<RangeType>::max();
		}
		return value >= 0 && value <= std::numeric_limits<RangeType>::max();
	}

	template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
	template<class... Ts> overload(Ts...)->overload<Ts...>;

	std::shared_ptr<base_type> get_smallest_viable_number_type(std::variant<std::uint64_t, double> variant, bool is_unsigned)
	{
		return get_base_type_from_type(std::visit(overload{
				[](const double value) -> internal_type {
					if (is_in_numerical_limits<float, double>(value))
					{
						return internal_type::t_f32;
					}

					if (is_in_numerical_limits<double, double>(value))
					{
						return internal_type::t_f64;
					}
					return internal_type::undefined;
				},
				[is_unsigned](const std::uint64_t value) -> internal_type {
					if (is_unsigned)
					{
						if (is_in_numerical_limits<std::uint8_t, std::uint64_t>(value))
						{
							return internal_type::t_u8;
						}

						if (is_in_numerical_limits<std::uint16_t, std::uint64_t>(value))
						{
							return internal_type::t_u16;
						}

						if (is_in_numerical_limits<std::uint32_t, std::uint64_t>(value))
						{
							return internal_type::t_u32;
						}

						if (is_in_numerical_limits<std::uint64_t, std::uint64_t>(value))
						{
							return internal_type::t_u64;
						}
						return internal_type::undefined;
					}

					if (is_in_numerical_limits<std::int8_t, std::uint64_t>(value))
					{
						return internal_type::t_i8;
					}

					if (is_in_numerical_limits<std::int16_t, std::uint64_t>(value))
					{
						return internal_type::t_i16;
					}

					if (is_in_numerical_limits<std::int32_t, std::uint64_t>(value))
					{
						return internal_type::t_i32;
					}

					if (is_in_numerical_limits<std::int64_t, std::uint64_t>(value))
					{
						return internal_type::t_i64;
					}

					return internal_type::undefined;
				}
			}, variant), false);
	}
}
