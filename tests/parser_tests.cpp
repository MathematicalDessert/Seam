#include <catch.hpp>
#include <iostream>
#include <parser/parser.h>
#include <ast/print_visitor.h>

TEST_CASE("test asdasdasd") {
	const std::wstring raw_source = LR"(
		fn test() -> none {
			{
				let b := true
			}

			let a: string = "asd2"
		}
	)";
	const auto source = std::make_unique<seam::Source>(raw_source);

	seam::Lexer lexer(source.get());
	const auto parser = new seam::Parser(std::unique_ptr<seam::Lexer>(&lexer));

	REQUIRE_NOTHROW([&]() {
		const	auto ast = parser->parse();

		seam::ast::PrintVisitor visitor;
		ast->accept(visitor);

		std::wcout << visitor.str() << std::endl;
	}());
}