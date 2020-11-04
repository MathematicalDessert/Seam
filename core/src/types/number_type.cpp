#include "number_type.hpp"

namespace seam::core::types
{
	number_type::number_type(const internal_type type, const std::size_t size) :
		base_type(type), size_(size)
	{}

	std::size_t number_type::get_size() const
	{
		return size_;
	}

	bool number_type::is_signed() const
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

	bool number_type::is_unsigned() const
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

	bool number_type::is_float() const
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

	bool number_type::is_integer() const
	{
		return !is_float();
	}

	llvm::Type* number_type::get_llvm_type(llvm::LLVMContext& context)
	{
		switch (size_)
		{
			case 8: return llvm::Type::getInt8Ty(context);
			case 16: return llvm::Type::getInt16Ty(context);
			case 32: return llvm::Type::getInt32Ty(context);
			case 64: return llvm::Type::getInt64Ty(context);
			default: return nullptr;
		}
	}

	bool number_type::operator==(const base_type& other_type)
	{
		return type_ == other_type.get_type();
	}
}