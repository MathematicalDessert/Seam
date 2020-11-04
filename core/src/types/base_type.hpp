#pragma once

#include <memory>
#include <string>
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
	
	class base_type : public std::enable_shared_from_this<base_type>
	{
	protected:
		std::string name_;
		const internal_type type_;
	public:
		explicit base_type(internal_type type);
		virtual ~base_type() = default;

		std::string get_name() const;
		internal_type get_type() const;

		bool is_void() const;
		bool is_number() const;
		bool is_boolean() const;
		bool is_string() const;
		bool is_class() const;
		bool is_alias() const;

		std::shared_ptr<base_type> get_base_type();

		virtual llvm::Type* get_llvm_type(llvm::LLVMContext& context) = 0;

		virtual bool operator==(const base_type& other_type) = 0;
	};
}
