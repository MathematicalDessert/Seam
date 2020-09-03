#include "parser.hpp"

namespace seam::core::parser
{
	parser::parser(const std::string_view source) :
		lexer_({ source })
	{}

}