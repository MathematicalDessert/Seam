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
		OpBitwiseAnd, // &
		OpLogicalAnd, // &&
		Arrow, // ->
		Colon, // :
		ColonEquals, // :=
		SymbOpenParen, // (
		SymbCloseParen, // )
		SymbOpenBrace, // {
		SymbCloseBrace, // }
		SymbComma, // ,
		

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
		case SymbolType::None: return L"<none>";
		case SymbolType::Identifier: return L"<identifier>";
		case SymbolType::StringLiteral: return L"<string_literal>";
		case SymbolType::NumberLiteral: return L"<number_literal>";
		case SymbolType::OpAdd: return L"+";
		case SymbolType::OpAddEq: return L"+=";
		case SymbolType::OpIncrement: return L"++";
		case SymbolType::OpSub: return L"-";
		case SymbolType::OpSubEq: return L"-=";
		case SymbolType::OpDecrement: return L"--";
		case SymbolType::OpDiv: return L"/";
		case SymbolType::OpMul: return L"*";
		case SymbolType::OpAssign: return L"=";
		case SymbolType::OpEq: return L"==";
		case SymbolType::OpBitwiseAnd: return L"&";
		case SymbolType::OpLogicalAnd: return L"&&";
		case SymbolType::Arrow: return L"->";
		case SymbolType::Colon: return L":";
		case SymbolType::ColonEquals: return L":=";
		case SymbolType::SymbOpenParen: return L"(";
		case SymbolType::SymbCloseParen: return L")";
		case SymbolType::SymbOpenBrace: return L"{";
		case SymbolType::SymbCloseBrace: return L"}";
		case SymbolType::SymbComma: return L",";
		case SymbolType::KeywordLet: return L"let";
		case SymbolType::KeywordFn: return L"fn";
		case SymbolType::KeywordType: return L"type";
		case SymbolType::KeywordWhile: return L"while";
		case SymbolType::KeywordFor: return L"for";
		case SymbolType::KeywordTrue: return L"true";
		case SymbolType::KeywordFalse: return L"false";
		case SymbolType::KeywordImport: return L"import";
		case SymbolType::KeywordIf: return L"if";
		case SymbolType::KeywordElse: return L"else";
		case SymbolType::KeywordElseIf: return L"elseif";
		}
	}

	template<const SymbolType T>
	constexpr auto symbol_type_to_name_cexpr() {
		switch (T) {
		case SymbolType::None: return L"<none>";
		case SymbolType::Identifier: return L"<identifier>";
		case SymbolType::StringLiteral: return L"<string_literal>";
		case SymbolType::NumberLiteral: return L"<number_literal>";
		case SymbolType::OpAdd: return L"+";
		case SymbolType::OpAddEq: return L"+=";
		case SymbolType::OpIncrement: return L"++";
		case SymbolType::OpSub: return L"-";
		case SymbolType::OpSubEq: return L"-=";
		case SymbolType::OpDecrement: return L"--";
		case SymbolType::OpDiv: return L"/";
		case SymbolType::OpMul: return L"*";
		case SymbolType::OpAssign: return L"=";
		case SymbolType::OpEq: return L"==";
		case SymbolType::OpBitwiseAnd: return L"&";
		case SymbolType::OpLogicalAnd: return L"&&";
		case SymbolType::Arrow: return L"->";
		case SymbolType::Colon: return L":";
		case SymbolType::ColonEquals: return L":=";
		case SymbolType::SymbOpenParen: return L"(";
		case SymbolType::SymbCloseParen: return L")";
		case SymbolType::SymbOpenBrace: return L"{";
		case SymbolType::SymbCloseBrace: return L"}";
		case SymbolType::SymbComma: return L",";
		case SymbolType::KeywordLet: return L"let";
		case SymbolType::KeywordFn: return L"fn";
		case SymbolType::KeywordType: return L"type";
		case SymbolType::KeywordWhile: return L"while";
		case SymbolType::KeywordFor: return L"for";
		case SymbolType::KeywordTrue: return L"true";
		case SymbolType::KeywordFalse: return L"false";
		case SymbolType::KeywordImport: return L"import";
		case SymbolType::KeywordIf: return L"if";
		case SymbolType::KeywordElse: return L"else";
		case SymbolType::KeywordElseIf: return L"elseif";
		}
	}
}
