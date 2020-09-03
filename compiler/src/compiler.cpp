#include <iostream>

#include "lexer/lexer.hpp"
#include "utils/exception.hpp"

int main()
{
	try
	{
		seam::core::lexer::lexer analyzer(R"(
import io

io.println("hello")
)");

		auto current_lexeme = analyzer.next_lexeme();
		while (current_lexeme.type != seam::core::lexer::lexeme_type::eof)
		{
			std::cout << current_lexeme.to_string().c_str() << std::endl;
			current_lexeme = analyzer.next_lexeme();
		}
	}
	catch (seam::core::utils::lexical_exception& error)
	{
		printf("%llu:%llu: %s\n", error.position_.line, error.position_.column, error.what());
	}
	return 0;
}
