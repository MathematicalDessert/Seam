#include <iostream>

#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "utils/exception.hpp"

int main()
{
	try
	{
		seam::core::parser::parser parser(R"(
		type int = i32

		type asdf
		{
			age: i32
		}

		fn asd(a: int) -> int @constructor
		{
			print(a)
			b := 1
			{
				print(a)
				print(b)
				c := 4
			}

			if (a > 2)
			{
				print("wot")
			}
			elseif (a < 2)
			{
				print("hi")
			}	
			else
			{
				print("heya")
			}

			while (true)
			{
				print("Hi!")
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
