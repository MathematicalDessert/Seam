#include <catch.hpp>
#include <lexer.h>

#include <exception.h>

TEST_CASE("lexing empty") {
	const std::string raw_source;
	const auto source = std::make_unique<seam::Source>(raw_source);
	seam::Lexer lexer(source.get());

	REQUIRE(lexer.peek() == seam::SymbolType::None);
	REQUIRE(lexer.next()->lexeme.empty());
}

TEST_CASE("ignore preceeding whitespace") {
	const std::string raw_source = "  \"Hello World!\"";
	const auto source = std::make_unique<seam::Source>(raw_source);

	SECTION("whitespace preceeding string") {
		seam::Lexer lexer(source.get());

		REQUIRE(lexer.peek() == seam::SymbolType::StringLiteral);
		auto token = lexer.next();
		REQUIRE(token->lexeme == "Hello World!");
		REQUIRE(token->position.start_idx == 0);
		REQUIRE(token->position.end_idx == 13);
	}
}

TEST_CASE("lexing strings works correctly") {
	const std::string raw_source = "\"Hello World!\"";
	const auto source = std::make_unique<seam::Source>(raw_source);

	const std::string bad_raw_source = "\"Hello World!";
	const auto bad_source = std::make_unique<seam::Source>(bad_raw_source);

	SECTION("inline string") {
		seam::Lexer lexer(source.get());

		REQUIRE(lexer.peek() == seam::SymbolType::StringLiteral);
		auto token = lexer.next();
		REQUIRE(token->lexeme == "Hello World!");
		REQUIRE(token->position.start_idx == 0);
		REQUIRE(token->position.end_idx == 13);
	}

	SECTION("bad string source") {
		seam::Lexer lexer(bad_source.get());

		REQUIRE_THROWS_WITH(lexer.next(), "expected \" but got EOF");
	}
}