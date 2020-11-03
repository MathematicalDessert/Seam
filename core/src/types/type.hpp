#pragma once

#include <string>
#include <variant>
#include <unordered_map>
#include <memory>
#include <sstream>
#include <llvm/IR/Type.h>
#include <llvm/IR/IRBuilder.h>

namespace seam::core::types
{
	enum class internal_type
	{
		undefined,
		t_void,
		t_bool,
		t_string,
		t_i8,
		t_i16,
		t_i32,
		t_i64,
		t_u8,
		t_u16,
		t_u32,
		t_u64,
		t_f32,
		t_f64,
		t_alias,
		t_class,
		count,
	};

	//inline internal_type get_type_from_name(const std::string& type_name);
	//inline std::string_view get_typename_from_internal_type(internal_type type);
	

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

	inline internal_type get_type_from_name(const std::string& type_name)
	{
		return built_in_type_map.at(type_name);
	}

	inline std::string_view get_typename_from_internal_type(const internal_type type)
	{
		return built_in_type_name_array[static_cast<int>(type)];
	}
	
	// nullable is a property of the variable, and NOT the type.
	// could also check constraints _on_ type classes themselves.
	
	struct base_type : public std::enable_shared_from_this<base_type>
	{
	protected:
		std::string name_; // internal name of type (e.g. t_i32, t_u16, User, ...)
		const internal_type type_; // internal type match
	public:
		explicit base_type(const internal_type type) :
			type_(type)
		{}

		[[nodiscard]] std::string get_name() const
		{
			if (!name_.empty())
			{
				return name_;
			}
			
			return std::string {get_typename_from_internal_type(type_)};
		}

		[[nodiscard]] internal_type get_type() const
		{
			return type_;
		}
		
		[[nodiscard]] bool is_number() const
		{
			return is_number(type_);
		}

		[[nodiscard]] bool is_boolean() const
		{
			return is_boolean(type_);
		}

		[[nodiscard]] static bool is_boolean(const internal_type type)
		{
			return type == internal_type::t_bool;
		}
		
		[[nodiscard]] static bool is_number(const internal_type type)
		{
			switch (type)
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

		[[nodiscard]] std::shared_ptr<base_type> get_base_type()
		{
			return this->shared_from_this();
		}

		virtual llvm::Type* get_llvm_type(llvm::LLVMContext& context) = 0;
	};
	
	struct void_type : public base_type
	{
		explicit void_type() :
			base_type(internal_type::t_void)
		{}
		
		llvm::Type* get_llvm_type(llvm::LLVMContext& context) override
		{
			return llvm::Type::getVoidTy(context);
		}
	};

	struct bool_type : public base_type
	{
		explicit bool_type() :
			base_type(internal_type::t_bool)
		{}
		
		llvm::Type* get_llvm_type(llvm::LLVMContext& context) override
		{
			return llvm::Type::getInt1Ty(context);
		}
	};

	struct string_type : public base_type
	{
		explicit string_type() :
			base_type(internal_type::t_string)
		{}
		
		llvm::Type* get_llvm_type(llvm::LLVMContext& context) override
		{
			return nullptr; // TODO: String types
		}
	};
	
	struct number_type : public base_type
	{
		const std::size_t size;
	public:
		number_type(const internal_type type, const std::size_t size = 0) :
			base_type(type), size(size)
		{}

		[[nodiscard]] std::size_t get_size() const
		{
			return size;
		}

		[[nodiscard]] bool is_signed() const
		{
			switch (type_)
			{
				case internal_type::t_i8:
				case internal_type::t_i16:
				case internal_type::t_i32:
				case internal_type::t_i64:
				{
					return true;
				}
				default:
				{
					return false;
				}
			}
		}

		[[nodiscard]] bool is_unsigned() const
		{
			switch (type_)
			{
				case internal_type::t_u8:
				case internal_type::t_u16:
				case internal_type::t_u32:
				case internal_type::t_u64:
				{
					return true;
				}
				default:
				{
					return false;
				}
			}
		}

		[[nodiscard]] bool is_float() const
		{
			switch (type_)
			{
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

		[[nodiscard]] bool is_integer() const
		{
			return !is_float();
		}
		
		llvm::Type* get_llvm_type(llvm::LLVMContext& context) override
		{
			switch (size)
			{
				case 8: return llvm::Type::getInt8Ty(context);
				case 16: return llvm::Type::getInt16Ty(context);
				case 32: return llvm::Type::getInt32Ty(context);
				case 64: return llvm::Type::getInt64Ty(context);
				default: return nullptr;
			}
		}
	};
	
	struct class_type : public base_type
	{
		std::unordered_map<std::string, std::shared_ptr<base_type>> members;
	public:
		void add_member(const std::string& member_name, const std::shared_ptr<base_type>& member_type)
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

		bool member_exists(const std::string& member_name)
		{
			return members.find(member_name) != members.cend();
		}
		
		base_type* get_member_type(const std::string& member_name)
		{
			if (const auto member_type = members.find(member_name); member_type != members.cend())
			{
				return member_type->second.get();
			}
			return nullptr;
		}
	};

	struct type_alias : public base_type
	{
		std::shared_ptr<base_type> alias_type;
	public:
		type_alias() :
			base_type(internal_type::t_alias)
		{}
		
		std::shared_ptr<base_type> get_alias_type() const
		{
			return alias_type;
		}
	};

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

	inline std::shared_ptr<base_type> get_base_type_from_name(const std::string& type_name, const bool safe = true)
	{
		const auto pair = built_in_types.find(type_name);
		if (pair == built_in_types.cend() && safe)
		{
			return nullptr;
		}
		return pair->second;
	}
}
