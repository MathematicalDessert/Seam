#pragma once

#include <stdexcept>

#include "position.hpp"

namespace seam::core::utils
{
	struct lexical_exception final : std::runtime_error
	{
		position position_;
		
		explicit lexical_exception(const position position, const std::string& msg) :
			std::runtime_error(msg), position_(position)
		{}

		explicit lexical_exception(const std::size_t line, const std::size_t column, const std::string& msg) :
			std::runtime_error(msg), position_({ line, column })
		{}
	};
}
