#include <catch2/catch.hpp>
#include <parser/lexer.h>

TEST_CASE("lexing empty") {
	const std::wstring raw_source;
	const auto source = std::make_unique<seam::Source>(raw_source);
	seam::Lexer lexer(source.get());

	REQUIRE(lexer.peek() == seam::TokenType::None);
	REQUIRE(lexer.next()->lexeme.empty());
}

TEST_CASE("double peeking") {
	const std::wstring raw_source = LR"("Hello World!")";
	const auto source = std::make_unique<seam::Source>(raw_source);

	seam::Lexer lexer(source.get());

	REQUIRE(lexer.peek() == seam::TokenType::StringLiteral);
	REQUIRE(lexer.peek() == seam::TokenType::StringLiteral);
	const auto token = lexer.next();
	REQUIRE(token->lexeme == L"Hello World!");
}

TEST_CASE("ignore preceding whitespace") {
	const std::wstring raw_source = LR"(            "Hello World!")";
	const auto source = std::make_unique<seam::Source>(raw_source);

	SECTION("whitespace preceding string") {
		seam::Lexer lexer(source.get());

		REQUIRE(lexer.peek() == seam::TokenType::StringLiteral);
		auto token = lexer.next();
		REQUIRE(token->lexeme == L"Hello World!");
		REQUIRE(token->position.start_idx == 12);
		REQUIRE(token->position.end_idx == 24);
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

    const auto bad_long_source = std::make_unique<seam::Source>(LR"(/// Bad Comment
"Following String")");

	SECTION("long comment") {
		seam::Lexer lexer(source.get());

		REQUIRE(lexer.peek() == seam::TokenType::StringLiteral);
		const auto token = lexer.next();
		REQUIRE(token->lexeme == L"Following String");
	}

	SECTION("short comment") {
		seam::Lexer lexer(short_source.get());

		REQUIRE(lexer.peek() == seam::TokenType::StringLiteral);
		const auto token = lexer.next();
		REQUIRE(token->lexeme == L"Following String");
	}

	SECTION("lex bad comment") {
        seam::Lexer lexer(bad_long_source.get());
        REQUIRE_THROWS_WITH(lexer.next(), "expected /// but got WEOF");
	}
}

TEST_CASE("lexing strings works correctly") {
	const std::wstring raw_source = LR"("Hello World!")";
	const auto source = std::make_unique<seam::Source>(raw_source);

	const std::wstring bad_raw_source = L"\"Hello World!";
	const auto bad_source = std::make_unique<seam::Source>(bad_raw_source);

	SECTION("inline string") {
		seam::Lexer lexer(source.get());

		REQUIRE(lexer.peek() == seam::TokenType::StringLiteral);
		const auto token = lexer.next();
		REQUIRE(token->lexeme == L"Hello World!");
		REQUIRE(token->position.start_idx == 0);
		REQUIRE(token->position.end_idx == 12);
	}

	SECTION("bad string source") {
		seam::Lexer lexer(bad_source.get());

		REQUIRE_THROWS_WITH(lexer.next(), "expected \" but got WEOF");
	}
}

TEST_CASE("lexing number literals works correctly") {
	// TODO: Test negative integers!

	const auto well_formed_integer = std::make_unique<seam::Source>(LR"(123)");
	const auto malformed_formed_integer = std::make_unique<seam::Source>(LR"(123X)");
    const auto malformed_formed_float_2 = std::make_unique<seam::Source>(LR"(.1p)");

	const auto well_formed_float = std::make_unique<seam::Source>(LR"(123.234 .32 0.89)");
	const auto malformed_float = std::make_unique<seam::Source>(LR"(1.2.3)");

	const auto well_formed_hex_integer = std::make_unique<seam::Source>(LR"(0xDEADBEEF)");
    const auto malformed_formed_hex_integer = std::make_unique<seam::Source>(LR"(0xBANANADEADBEEF)");
    const auto malformed_formed_hex_integer_2 = std::make_unique<seam::Source>(LR"(0xX)");

    SECTION("lex well formed integer") {
		seam::Lexer lexer(well_formed_integer.get());

		REQUIRE(lexer.peek() == seam::TokenType::NumberLiteral);
		const auto token = lexer.next();
		REQUIRE(token->lexeme == L"123");
		REQUIRE(token->position.start_idx == 0);
		REQUIRE(token->position.end_idx == 2);
	}

	SECTION("lex malformed integer") {
		seam::Lexer lexer(malformed_formed_integer.get());

		REQUIRE_THROWS_WITH(lexer.next(), "expected digit but got 'X'");
	}

	SECTION("lex well formed floats") {
		seam::Lexer lexer(well_formed_float.get());

		for (auto i = 0; i < 3; i++) {
			REQUIRE(lexer.peek() == seam::TokenType::NumberLiteral);
			const auto token = lexer.next();

			switch (i) {
			case 0: {
				REQUIRE(token->lexeme == L"123.234");
				REQUIRE(token->position.start_idx == 0);
				REQUIRE(token->position.end_idx == 6);
				break;
			}
			case 1: {
				REQUIRE(token->lexeme == L".32");
				REQUIRE(token->position.start_idx == 8);
				REQUIRE(token->position.end_idx == 10);
				break;
			}
			case 2: {
				REQUIRE(token->lexeme == L"0.89");
				REQUIRE(token->position.start_idx == 12);
				REQUIRE(token->position.end_idx == 15);
				break;
			}
			default: FAIL(); break; // should never reach this.
			}
		}
	}

	SECTION("lex malformed float") {
		seam::Lexer lexer(malformed_float.get());

		REQUIRE_THROWS_WITH(lexer.next(), "malformed floating point number: a float can only have one point");
	}

	SECTION("lex well formed hex integer") {
		seam::Lexer lexer(well_formed_hex_integer.get());

		REQUIRE(lexer.peek() == seam::TokenType::NumberLiteral);
		const auto token = lexer.next();
		REQUIRE(token->lexeme == L"0xDEADBEEF");
		REQUIRE(token->position.start_idx == 0);
		REQUIRE(token->position.end_idx == 9);
	}

	SECTION("lex malformed hex integer") {
		seam::Lexer lexer(malformed_formed_hex_integer.get());

		REQUIRE_THROWS_WITH(lexer.next(), "expected hex-digit but got 'N'");
	}

    SECTION("lex malformed hex integer 2") {
        seam::Lexer lexer(malformed_formed_hex_integer_2.get());

        REQUIRE_THROWS_WITH(lexer.next(), "malformed number literal");
    }

    SECTION("lex malformed float 2") {
        seam::Lexer lexer(malformed_formed_float_2.get());

        REQUIRE_THROWS_WITH(lexer.next(), "expected digit but got 'p'");
    }
}

TEST_CASE("lexing identifiers works correctly") {
	const auto identifier = std::make_unique<seam::Source>(LR"(this_is_not_a_keyword thisIsAlsoAKeyword AnotherIdentifier _Identifier _1IdentifierWithNumber Identifier_with_Number1)");
	seam::Lexer lexer(identifier.get());

	for (auto i = 0; i < 6; i++) {
		const auto token = lexer.next();

		switch (i) {
		case 0: {
			REQUIRE(token->lexeme == L"this_is_not_a_keyword");
			break;
		}
		case 1: {
			REQUIRE(token->lexeme == L"thisIsAlsoAKeyword");
			break;
		}
		case 2: {
			REQUIRE(token->lexeme == L"AnotherIdentifier");
			break;
		}
		case 3: {
			REQUIRE(token->lexeme == L"_Identifier");
			break;
		}
		case 4: {
			REQUIRE(token->lexeme == L"_1IdentifierWithNumber");
			break;
		}
		case 5: {
			REQUIRE(token->lexeme == L"Identifier_with_Number1");
			break;
		}
		default: FAIL(); break; // should never reach this.
		}
	}
}

TEST_CASE("lexing keywords works correctly") {
	const auto identifier = std::make_unique<seam::Source>(LR"(let variable fn variable_again if)");
	seam::Lexer lexer(identifier.get());

	REQUIRE(lexer.peek() == seam::TokenType::KeywordLet);
	lexer.next();

	REQUIRE(lexer.peek() == seam::TokenType::Identifier);
	lexer.next();

	REQUIRE(lexer.peek() == seam::TokenType::KeywordFn);
	lexer.next();

	REQUIRE(lexer.peek() == seam::TokenType::Identifier);
	lexer.next();

	REQUIRE(lexer.peek() == seam::TokenType::KeywordIf);
	lexer.next();
}

TEST_CASE("lexing symbols") {
	const auto identifier = std::make_unique<seam::Source>(LR"(-> ++ + - --- -++ +-+-)");
	seam::Lexer lexer(identifier.get());

	for (auto i = 0; i < 12; i++) {
		switch (i) {
		case 0: REQUIRE(lexer.peek() == seam::TokenType::Arrow); break;
		case 1: REQUIRE(lexer.peek() == seam::TokenType::OpIncrement); break;
		case 2: REQUIRE(lexer.peek() == seam::TokenType::OpAdd); break;
		case 3: REQUIRE(lexer.peek() == seam::TokenType::OpSub); break;
		case 4: REQUIRE(lexer.peek() == seam::TokenType::OpDecrement); break;
		case 5: REQUIRE(lexer.peek() == seam::TokenType::OpSub); break;
		case 6: REQUIRE(lexer.peek() == seam::TokenType::OpSub); break;
		case 7: REQUIRE(lexer.peek() == seam::TokenType::OpIncrement); break;
		case 8: REQUIRE(lexer.peek() == seam::TokenType::OpAdd); break;
		case 9: REQUIRE(lexer.peek() == seam::TokenType::OpSub); break;
		case 10: REQUIRE(lexer.peek() == seam::TokenType::OpAdd); break;
		case 11: REQUIRE(lexer.peek() == seam::TokenType::OpSub); break;
		default: FAIL(); break;
		}

		lexer.next();
	}

	SECTION("lex unknown symbol") {
        const auto id = std::make_unique<seam::Source>(LR"(~)");
        seam::Lexer lexer(id.get());

        REQUIRE_THROWS_WITH(lexer.next(), "unknown symbol found ~");
	}
}