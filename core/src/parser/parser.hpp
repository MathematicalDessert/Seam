#pragma once

#include "../lexer/lexer.hpp"

namespace seam::core::parser
{
	class parser
	{
		lexer::lexer lexer_;
	public:
		explicit parser(const std::string_view source);
	};
}
