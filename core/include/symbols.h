#include <string>
#include <unordered_map>

namespace seam {
	enum class SymbolType {
		None,
		Identifier,
		StringLiteral,
		NumberLiteral,

		PlusSign,
		MinusSign,
		ForwardSlash,

		KeywordLet,
		KeywordFn,
		KeywordType,
		KeywordWhile,
		KeywordFor,
		KeywordTrue,
		KeywordFalse,
		KeywordImport,
		KeywordIf,
		KeywordElse,
		KeywordElseIf,
	};

	const std::unordered_map<std::wstring, SymbolType> str_to_keyword_map {
		{ L"let", SymbolType::KeywordLet },
		{ L"fn", SymbolType::KeywordFn },
		{ L"type", SymbolType::KeywordType },
		{ L"while", SymbolType::KeywordWhile },
		{ L"for", SymbolType::KeywordFor },
		{ L"true", SymbolType::KeywordTrue },
		{ L"false", SymbolType::KeywordFalse },
		{ L"import", SymbolType::KeywordImport },
		{ L"if", SymbolType::KeywordIf },
		{ L"else", SymbolType::KeywordElse },
		{ L"elseif", SymbolType::KeywordElseIf }
	};
}
