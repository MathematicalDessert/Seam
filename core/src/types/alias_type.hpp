#pragma once

#include "base_type.hpp"

namespace seam::core::types
{
	class alias_type final : public base_type
	{
		std::shared_ptr<base_type> aliased_type;
	public:
		explicit alias_type(std::shared_ptr<base_type> aliased_type);

		bool is_void() const override;
		bool is_number() const override;
		bool is_boolean() const override;
		bool is_string() const override;
		bool is_class() const override;
		bool is_alias() const override;
		
		std::shared_ptr<base_type> get_aliased_type() const;

		llvm::Type* get_llvm_type(llvm::LLVMContext& context) override;
		
		bool operator==(const base_type& other_type) override;
	};
}