#pragma once

#include "base_type.hpp"

namespace seam::core::types
{
	class alias_type final : public base_type
	{
		std::shared_ptr<base_type> aliased_type;
	public:
		explicit alias_type();

		std::shared_ptr<base_type> get_aliased_type() const;

		bool operator==(const base_type& other_type) override;
	};
}