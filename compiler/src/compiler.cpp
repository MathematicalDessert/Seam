#include <iostream>

#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "utils/exception.hpp"

int main()
{
	try
	{
		seam::core::parser::parser parser(R"(
fn asd(a: i32) -> i32 @constructor
{
	print(a)
	b := 1
	{
		print(a)
		print(b)
		c := 4
	}
	return b + 2
}
)");

		parser.parse();
	}
	catch (seam::core::utils::lexical_exception& error)
	{
		printf("[LEXER ERROR] %llu:%llu: %s\n", error.position_.line, error.position_.column, error.what());
	}
	catch (seam::core::utils::parser_exception& error)
	{
		printf("[PARSER ERROR] %llu:%llu: %s\n", error.position_.line, error.position_.column, error.what());
	}
	return 0;
}
