#include <iostream>

#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "utils/exception.hpp"
#include "module.hpp"

int main()
{
	try
	{
		 auto main_module = std::make_unique< seam::core::module>("main",
			R"(
fn main() @constructor
{
	a: i32
	a = 123
}
)"
		);
		
		seam::core::parser::parser parser(main_module.get());
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