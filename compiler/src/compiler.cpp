#include <iostream>

#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "utils/exception.hpp"
#include "module.hpp"
#include "code_generation/code_generator.hpp"
#include <llvm/Support/WithColor.h>
#include <llvm/Bitcode/BitcodeWriter.h>
#include <llvm/Support/Program.h>

int main()
{
	try
	{
		 auto main_module = std::make_unique< seam::core::module>("main",
			R"(
fn test(a: i32) -> i32
{
	if (a <= 1)
	{
		return 1
	}
	return 2
}

fn main() -> void @constructor
{
	a: i32 = 1 + 2// Forward declaration
	a = a + 3
}
)"
		);
		
		seam::core::parser::parser parser(main_module.get());
		parser.parse();

		seam::core::code_generation::code_generator code_gen(main_module.get());
		auto module = code_gen.generate();

		module->print(llvm::outs(), nullptr);
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