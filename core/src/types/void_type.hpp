#pragma once

#include "base_type.hpp"

namespace seam::core::types
{
	class void_type final : public base_type
	{
	public:
		explicit void_type();

		// llvm type
		llvm::Type* get_llvm_type(llvm::LLVMContext& context) override;

		// Operator overloads
		bool operator==(const base_type& other_type) override;
	};
}
