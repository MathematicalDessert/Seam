#pragma once

#include <optional>
#include <string>
#include <variant>
#include <type_traits>

namespace seam::core::types
{
	enum class built_in_type
	{
		undefined,
		void_,
		bool_,
		string,
		u8,
		i8,		
		u16,
		i16,
		u32,
		i32,
		u64,
		i64,
		f32,
		f64,
		count_,
	};

	const std::unordered_map<std::string, built_in_type> built_in_type_map = {
		{ "void", built_in_type::void_ },
		{ "bool", built_in_type::bool_ },
		{ "string", built_in_type::string },
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

	constexpr static std::string_view built_in_type_name_array[] = {
		"<undefined>",
		"void",
		"bool",
		"string",
		"u8",
		"i8",
		"u16",
		"i16",
		"u32",
		"i32",
		"u64",
		"i64",
		"f32",
		"f64",
		"<count>",
	};
	static_assert(static_cast<int>(built_in_type::count_) + 1 == std::size(built_in_type_name_array), "built_in_type and built_in_type_name_array size mismatch");

	inline bool is_signed_integer_type(const built_in_type type)
	{
		switch (type)
		{
			case built_in_type::i8:
			case built_in_type::i16:
			case built_in_type::i32:
			case built_in_type::i64:
			{
				return true;
			}
			default:
			{
				return false;
			}
		}
	}

	inline bool is_unsigned_integer_type(const built_in_type type)
	{
		switch (type)
		{
			case built_in_type::u8:
			case built_in_type::u16:
			case built_in_type::u32:
			case built_in_type::u64:
			{
				return true;
			}
			default:
			{
				return false;
			}
		}
	}

	inline bool is_float_type(const built_in_type type)
	{
		switch (type)
		{
			case built_in_type::f32:
			case built_in_type::f64:
			{
				return true;
			}
			default:
			{
				return false;
			}
		}
	}

	template<typename RangeType, typename ValueType>
	inline bool is_in_numerical_limits(ValueType value)
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

	inline built_in_type get_smallest_viable_number_type(std::variant<std::uint64_t, double> variant, bool is_unsigned = false)
	{
		return std::visit(overload{
				[](double value) -> types::built_in_type {
					if (is_in_numerical_limits<float, double>(value))
					{
						return types::built_in_type::f32;
					}

					if (is_in_numerical_limits<double, double>(value))
					{
						return types::built_in_type::f64;
					}
					return built_in_type::undefined;
				},
				[is_unsigned](std::uint64_t value) -> types::built_in_type {
					if (is_unsigned)
					{
						if (is_in_numerical_limits<std::uint8_t, std::uint64_t>(value))
						{
							return types::built_in_type::u8;
						}

						if (is_in_numerical_limits<std::uint16_t, std::uint64_t>(value))
						{
							return types::built_in_type::u16;
						}

						if (is_in_numerical_limits<std::uint32_t, std::uint64_t>(value))
						{
							return types::built_in_type::u32;
						}

						if (is_in_numerical_limits<std::uint64_t, std::uint64_t>(value))
						{
							return types::built_in_type::u64;
						}
						return built_in_type::undefined;
					}

					if (is_in_numerical_limits<std::int8_t, std::uint64_t>(value))
					{
						return types::built_in_type::i8;
					}

					if (is_in_numerical_limits<std::int16_t, std::uint64_t>(value))
					{
						return types::built_in_type::i16;
					}

					if (is_in_numerical_limits<std::int32_t, std::uint64_t>(value))
					{
						return types::built_in_type::i32;
					}

					if (is_in_numerical_limits<std::int64_t, std::uint64_t>(value))
					{
						return types::built_in_type::i64;
					}

					return built_in_type::undefined;
				}
			}, variant);
	}

	inline int get_number_bytes_size(const built_in_type type)
	{
		switch (type)
		{
			case built_in_type::u8:
			case built_in_type::i8:
			{
				return 1;
			}
			case built_in_type::u16:
			case built_in_type::i16:
			{
				return 2;
			}
			case built_in_type::u32:
			case built_in_type::i32:
			{
				return 4;
			}
			case built_in_type::u64:
			case built_in_type::i64:
			{
				return 8;
			}
			default:
			{
				return -1;
			}
		}
	}

	inline bool is_integer(const built_in_type type)
	{
		return is_signed_integer_type(type) || is_unsigned_integer_type(type);
	}

	inline bool is_number(const built_in_type type)
	{
		return is_signed_integer_type(type) || is_unsigned_integer_type(type) || is_float_type(type);
	}

	inline std::optional<built_in_type> string_to_type(const std::string& type)
	{
		if (const auto iterator = built_in_type_map.find(type); iterator != built_in_type_map.cend())
		{
			return iterator->second;
		}
		return {};
	}
	
	inline built_in_type coerce_type(built_in_type priority, built_in_type sub)
	{
		if (is_integer(priority) && is_integer(sub))
		{
			return static_cast<int>(priority) > static_cast<int>(sub) ? priority : sub;
		}
		return built_in_type::undefined;
	}

	struct type
	{
		std::string name;
		built_in_type internal_type = built_in_type::undefined;
		bool is_nullable = false;
		bool is_implicit_type = false;

		explicit type(std::string name, const bool is_nullable = false , const bool is_implicit = false) :
			name(std::move(name)), is_nullable(is_nullable), is_implicit_type(is_implicit)
		{
			auto potential_internal_type = built_in_type_map.find(this->name);
			if (potential_internal_type != built_in_type_map.cend())
			{
				internal_type = potential_internal_type->second;
			}
		}

		explicit type(built_in_type type) :
			internal_type(type)
		{}
	};
}
