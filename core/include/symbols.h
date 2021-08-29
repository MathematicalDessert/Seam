#pragma once

namespace seam {
	/**
	 * Symbol Types.
	 */
	enum class SymbolType {
		None,
		Identifier,
		StringLiteral,
		NumberLiteral,

		OpAdd, // +
		OpAddEq, // +=
		OpIncrement, // ++
		OpSub, // -
		OpSubEq, // -=
		OpDecrement, // --
		OpDiv, // /
		OpMul, // *
		OpAssign, // =
		OpEq, // ==
		Arrow, // ->
		Colon, // :
		ColonEquals, //
		SymbOpenParen, // (
		SymbCloseParen, // )
		SymbOpenBrace, // {
		SymbCloseBrace, // }
		

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


	static auto symbol_type_to_name(const SymbolType type) {
		switch (type) {
		case SymbolType::KeywordFn: return L"fn";
		case SymbolType::Identifier: return L"<identifier>";
		default: return L"UNKNOWN";
		}
	}

	template<const SymbolType T>
	constexpr auto symbol_type_to_name_cexpr() {
		switch (T) {
			case SymbolType::KeywordFn: return L"fn";
			case SymbolType::Identifier: return L"<identifier>";
			default: return L"UNKNOWN";
		}
	}
}
