#include <catch.hpp>
#include <lexer.h>

#include <exception.h>

TEST_CASE("lexing empty") {
	const std::wstring raw_source;
	const auto source = std::make_unique<seam::Source>(raw_source);
	seam::Lexer lexer(source.get());

	REQUIRE(lexer.peek() == seam::SymbolType::None);
	REQUIRE(lexer.next()->lexeme.empty());
}

TEST_CASE("double peeking") {
	const std::wstring raw_source = LR"("Hello World!")";
	const auto source = std::make_unique<seam::Source>(raw_source);

	seam::Lexer lexer(source.get());

	REQUIRE(lexer.peek() == seam::SymbolType::StringLiteral);
	REQUIRE(lexer.peek() == seam::SymbolType::StringLiteral);
	const auto token = lexer.next();
	REQUIRE(token->lexeme == L"Hello World!");
}

TEST_CASE("ignore preceeding whitespace") {
	const std::wstring raw_source = LR"(            "Hello World!")";
	const auto source = std::make_unique<seam::Source>(raw_source);

	SECTION("whitespace preceeding string") {
		seam::Lexer lexer(source.get());

		REQUIRE(lexer.peek() == seam::SymbolType::StringLiteral);
		auto token = lexer.next();
		REQUIRE(token->lexeme == L"Hello World!");
		REQUIRE(token->position.start_idx == 12);
		REQUIRE(token->position.end_idx == 25);
	}
}

TEST_CASE("lexing strings") {
	const std::wstring raw_source = LR"(/// Test Long Comment
	Another line in the comment.
	End line. ///

	"Following String")";
	const auto source = std::make_unique<seam::Source>(raw_source);
	const auto short_source = std::make_unique<seam::Source>(LR"(// Short Comment
"Following String")");

	SECTION("long comment") {
		seam::Lexer lexer(source.get());

		REQUIRE(lexer.peek() == seam::SymbolType::StringLiteral);
		const auto token = lexer.next();
		REQUIRE(token->lexeme == L"Following String");
	}

	SECTION("short comment") {
		seam::Lexer lexer(short_source.get());

		REQUIRE(lexer.peek() == seam::SymbolType::StringLiteral);
		const auto token = lexer.next();
		REQUIRE(token->lexeme == L"Following String");
	}
}

TEST_CASE("lexing strings works correctly") {
	const std::wstring raw_source = LR"("Hello World!")";
	const auto source = std::make_unique<seam::Source>(raw_source);

	const std::wstring bad_raw_source = L"\"Hello World!";
	const auto bad_source = std::make_unique<seam::Source>(bad_raw_source);

	SECTION("inline string") {
		seam::Lexer lexer(source.get());

		REQUIRE(lexer.peek() == seam::SymbolType::StringLiteral);
		const auto token = lexer.next();
		REQUIRE(token->lexeme == L"Hello World!");
		REQUIRE(token->position.start_idx == 0);
		REQUIRE(token->position.end_idx == 13);
	}

	SECTION("bad string source") {
		seam::Lexer lexer(bad_source.get());

		REQUIRE_THROWS_WITH(lexer.next(), "expected \" but got WEOF");
	}
}