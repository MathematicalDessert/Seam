#pragma once

#include "base_type.hpp"

namespace seam::core::types
{
	class string_type final : public base_type
	{
	public:
		explicit string_type();

		llvm::Type* get_llvm_type(llvm::LLVMContext& context) override;

		bool operator==(const base_type& other_type) override;
	};
}