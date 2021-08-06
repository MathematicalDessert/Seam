#pragma once

#include <string>

const std::wstring LEX_UNEXPECTED_WEOF_EXCEPTION_FMT = L"expected {} but got WEOF";

const std::wstring EXPECTED_BUT_GOT = L"expected {} but got '{}'";

// Number Lexing Exception String
const std::wstring LEX_MALFORMED_NUMBER_LITERAL = L"malformed number literal";
const std::wstring LEX_MALFORMED_HEX_NUMBER_LITERAL = L"malformed hex number literal";
const std::wstring LEX_MALFORMED_FLOATING_POINT_NUMBER_LITERAL = L"malformed floating point number: {}";

const std::wstring LEX_MALFORMED_FLOAT_TWO_POINTS = L"a float can only have one point";