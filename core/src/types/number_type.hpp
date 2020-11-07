#pragma once

#include "base_type.hpp"

namespace seam::core::types
{
	class number_type final : public base_type
	{
		const std::size_t size_;
	public:
		explicit number_type(internal_type type, std::size_t size = 0);

		// size of this number type
		std::size_t get_size() const;

		// number property methods
		bool is_signed() const;
		bool is_unsigned() const;
		bool is_float() const;
		bool is_integer() const;

		// llvm type
		llvm::Type* get_llvm_type(llvm::LLVMContext& context) override;

		// resize
		std::shared_ptr<base_type> resize(number_type* weak_binding_number);
		
		// Operator overloads
		bool operator==(const base_type& other_type) override;
	};
}
