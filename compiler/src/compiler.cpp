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
fn test(num1: i32, num2: i32)
{
	return num1 + num2 == num2 + num1
}

fn main()  @constructor
{
	test(1, 10)
	test(3, 99)
	return 1
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
		printf("[LEXER ERROR] %llu:%llu: %s\n", error.position.line, error.position.column, error.what());
	}
	catch (seam::core::utils::parser_exception& error)
	{
		printf("[PARSER ERROR] %llu:%llu: %s\n", error.position.line, error.position.column, error.what());
	}
	catch (seam::core::utils::compiler_exception& error)
	{
		printf("[COMPILER ERROR] %llu:%llu: %s\n", error.position.line, error.position.column, error.what());
	}
	catch (seam::core::utils::type_exception& error)
	{
		printf("[TYPE ERROR] %llu:%llu: %s\n", error.position.line, error.position.column, error.what());
	}
	return 0;
}