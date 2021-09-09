	#pragma once

#include "source_position.h"

namespace seam {
	/**
	 * Symbol Types.
	 */
	enum class TokenType {
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
		OpenParen, // (
		CloseParen, // )
		OpenBrace, // {
		CloseBrace, // }
		

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


	static auto token_type_to_name(const TokenType type) {
		switch (type) {
		case TokenType::None: return L"<none>";
		case TokenType::Identifier: return L"<identifier>";
		case TokenType::StringLiteral: return L"<string_literal>";
		case TokenType::NumberLiteral: return L"<number_literal>";
		case TokenType::OpAdd: return L"+";
		case TokenType::OpAddEq: return L"+=";
		case TokenType::OpIncrement: return L"++";
		case TokenType::OpSub: return L"-";
		case TokenType::OpSubEq: return L"-=";
		case TokenType::OpDecrement: return L"--";
		case TokenType::OpDiv: return L"/";
		case TokenType::OpMul: return L"*";
		case TokenType::OpAssign: return L"=";
		case TokenType::OpEq: return L"==";
		case TokenType::OpBitwiseAnd: return L"&";
		case TokenType::OpLogicalAnd: return L"&&";
		case TokenType::Arrow: return L"->";
		case TokenType::Colon: return L":";
		case TokenType::ColonEquals: return L":=";
		case TokenType::OpenParen: return L"(";
		case TokenType::CloseParen: return L")";
		case TokenType::OpenBrace: return L"{";
		case TokenType::CloseBrace: return L"}";
		case TokenType::KeywordLet: return L"let";
		case TokenType::KeywordFn: return L"fn";
		case TokenType::KeywordType: return L"type";
		case TokenType::KeywordWhile: return L"while";
		case TokenType::KeywordFor: return L"for";
		case TokenType::KeywordTrue: return L"true";
		case TokenType::KeywordFalse: return L"false";
		case TokenType::KeywordImport: return L"import";
		case TokenType::KeywordIf: return L"if";
		case TokenType::KeywordElse: return L"else";
		case TokenType::KeywordElseIf: return L"elseif";
		}
	}

	template<const TokenType T>
	constexpr auto token_type_to_name_cexpr() {
		switch (T) {
		case TokenType::None: return L"<none>";
		case TokenType::Identifier: return L"<identifier>";
		case TokenType::StringLiteral: return L"<string_literal>";
		case TokenType::NumberLiteral: return L"<number_literal>";
		case TokenType::OpAdd: return L"+";
		case TokenType::OpAddEq: return L"+=";
		case TokenType::OpIncrement: return L"++";
		case TokenType::OpSub: return L"-";
		case TokenType::OpSubEq: return L"-=";
		case TokenType::OpDecrement: return L"--";
		case TokenType::OpDiv: return L"/";
		case TokenType::OpMul: return L"*";
		case TokenType::OpAssign: return L"=";
		case TokenType::OpEq: return L"==";
		case TokenType::OpBitwiseAnd: return L"&";
		case TokenType::OpLogicalAnd: return L"&&";
		case TokenType::Arrow: return L"->";
		case TokenType::Colon: return L":";
		case TokenType::ColonEquals: return L":=";
		case TokenType::OpenParen: return L"(";
		case TokenType::CloseParen: return L")";
		case TokenType::OpenBrace: return L"{";
		case TokenType::CloseBrace: return L"}";
		case TokenType::KeywordLet: return L"let";
		case TokenType::KeywordFn: return L"fn";
		case TokenType::KeywordType: return L"type";
		case TokenType::KeywordWhile: return L"while";
		case TokenType::KeywordFor: return L"for";
		case TokenType::KeywordTrue: return L"true";
		case TokenType::KeywordFalse: return L"false";
		case TokenType::KeywordImport: return L"import";
		case TokenType::KeywordIf: return L"if";
		case TokenType::KeywordElse: return L"else";
		case TokenType::KeywordElseIf: return L"elseif";
		}
	}

    /**
     * Token Object.
     *
     * @note Immutable, this object should not be promoted
     * or demoted in any way.
     */
    struct Token {
        // token type
        const TokenType type;
        // token lexeme
        const std::wstring lexeme;
        // token position
        const SourcePosition position;

        Token(const TokenType type, std::wstring lexeme, const SourcePosition position)
                : type(type), lexeme(std::move(lexeme)), position(position) {}
    };
}
