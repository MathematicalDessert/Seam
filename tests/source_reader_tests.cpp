#include <catch2/catch.hpp>
#include <source.h>

TEST_CASE("source reader reads next characters", "[SourceReader]") {
	const std::wstring test_source_1 = L"The quick brown fox jumped over the lazy dog";
	const auto source = std::make_unique<seam::Source>(test_source_1);

	seam::SourceReader source_reader{
		source.get()
	};

	REQUIRE(source_reader.length() == test_source_1.length());
	REQUIRE(source_reader.start_pointer() == 0);
	REQUIRE(source_reader.read_pointer() == 0);

	SECTION("read whole string") {
		for (size_t it = 0; it < source_reader.length(); it++) {
			REQUIRE(source_reader.next_char() == test_source_1[it]);
		}
	}

	SECTION("read overflow string") {
		for (size_t it = 0; it < source_reader.length(); it++) {
			static_cast<void>(source_reader.next_char());
		}
		REQUIRE(source_reader.next_char() == WEOF);
	}
}

TEST_CASE("source reader consumes correctly", "[SourceReader]") {
	const std::wstring test_source_1 = L"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.";
	const auto source = std::make_unique<seam::Source>(test_source_1);

	seam::SourceReader source_reader{
		source.get()
	};

	REQUIRE(source_reader.length() == test_source_1.length());
	REQUIRE(source_reader.start_pointer() == 0);
	REQUIRE(source_reader.read_pointer() == 0);

	SECTION("read whole string & consume") {
		for (size_t it = 0; it < source_reader.length(); it++) {
			static_cast<void>(source_reader.next_char());
		}
		REQUIRE(source_reader.consume() == test_source_1);
	}

	SECTION("consume empty") {
		REQUIRE(source_reader.consume().empty());
	}

	SECTION("consume partial") {
		for (size_t it = 0; it < 5; it++) {
			static_cast<void>(source_reader.next_char());
		} 
		REQUIRE(source_reader.consume() == test_source_1.substr(0, 5));
	}
}

TEST_CASE("source reader peeks correctly", "[SourceReader]") {
	const std::wstring test_source_1 = L"The quick brown fox jumped over the lazy dog";
	const auto source = std::make_unique<seam::Source>(test_source_1);

	seam::SourceReader source_reader {
		source.get()
	};

	SECTION("peek, move, peek character") {
		// should get first character
		REQUIRE(source_reader.peek_char() == test_source_1[0]);
		// should also get first character
		REQUIRE(source_reader.next_char() == test_source_1[0]);
		// should get second character
		REQUIRE(source_reader.peek_char() == test_source_1[1]);
	}

	SECTION("peek at different indexes ahead") {
		REQUIRE(source_reader.peek_char(1) == test_source_1[1]);
	}

	SECTION("peek out of bounds") {
		REQUIRE(source_reader.peek_char(1000) == WEOF);
	}
}

TEST_CASE("get source string from source object", "[Source]") {
    const std::wstring test_source_1 = L"The quick brown fox jumped over the lazy dog";
    const auto source = std::make_unique<seam::Source>(test_source_1);

    seam::SourceReader source_reader {
            source.get()
    };

    REQUIRE(source->get() == test_source_1);
}

/*
TEST_CASE("source reader reads correctly", "[SourceReader]") {
	const std::wstring test_source_1 = L"The quick brown fox jumped over the lazy dog";
	const auto source = std::make_unique<seam::Source>(test_source_1);
	
	seam::SourceReader source_reader {
		source.get()
	};

	REQUIRE(source_reader.length() == test_source_1.length());
	REQUIRE(source_reader.start_pointer() == 0);
	REQUIRE(source_reader.read_pointer() == 0);

	SECTION("peeking first character") {
		REQUIRE(source_reader.peek_char() == test_source_1[0]);
	}

	SECTION("peek and next character") {
		// should get first character
		REQUIRE(source_reader.peek_char() == test_source_1[0]);
		// should also get first character
		REQUIRE(source_reader.next_char() == test_source_1[0]);
		// should get second character
		REQUIRE(source_reader.peek_char() == test_source_1[1]);
	}

	SECTION("peek/read -> consume") {
		const auto source_length = source_reader.length();
		
		for (size_t it = 0; it < source_length - 1; it++) {
			REQUIRE(source_reader.next_char() == test_source_1[it]);
			REQUIRE(source_reader.peek_char() == test_source_1[it + 1]);
		}
		
		REQUIRE(source_reader.consume() == test_source_1.substr(0, source_length - 1));
		
		REQUIRE(source_reader.next_char() == test_source_1[source_length - 1]);
		
		REQUIRE(source_reader.consume() == test_source_1.substr(source_length - 1, source_length));
	}

	SECTION("read overflow") {
		source_reader.discard(source_reader.length());
		
		REQUIRE(source_reader.peek_char() == WEOF);
		REQUIRE(source_reader.next_char() == WEOF);
	}

	SECTION("discard with length out of bound") {
		source_reader.discard(source_reader.length() + 90);

		REQUIRE(source_reader.peek_char() == WEOF);
		REQUIRE(source_reader.next_char() == WEOF);
	}

	SECTION("discard with length in bound") {
		const auto source_length = source_reader.length();
		source_reader.discard(source_length - 1);

		REQUIRE(source_reader.peek_char() == test_source_1[source_length - 1]);
		REQUIRE(source_reader.next_char() == test_source_1[source_length - 1]);
	}

	SECTION("discard without length") {
		const auto read_len = 6; // random number between 1 - (length - 1)
		
		for (auto i = 0; i < read_len; i++) {
			static_cast<void>(source_reader.next_char());
		}
		
		source_reader.discard();

		for (size_t i = 0; i < source_reader.length() - read_len; i++) {
			static_cast<void>(source_reader.next_char());
		}
		
		REQUIRE(source_reader.consume() == test_source_1.substr(read_len));
	}

	SECTION("get wchar_t out of bound") {
		REQUIRE(source_reader.get_char(10000) == WEOF);
	}

	SECTION("get wchar_t in bound") {
		REQUIRE(source_reader.get_char(2) == test_source_1[2]);
	}
}*/