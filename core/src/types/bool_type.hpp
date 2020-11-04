#pragma once

#include "base_type.hpp"

namespace seam::core::types
{
	class bool_type final : public base_type
	{
	public:
		explicit bool_type();

		llvm::Type* get_llvm_type(llvm::LLVMContext& context) override;

		bool operator==(const base_type& other_type) override;
	};
}