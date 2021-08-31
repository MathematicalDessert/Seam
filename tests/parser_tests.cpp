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

			if (true) {
				let a: string = "asd2"
			} elseif (false) {
				let c := 23
			} else {
				let d: number = 1234
			}

			while (true) {
				
			}
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