#include <string>
#include <unordered_map>

namespace seam {
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
		Arrow, // ->

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
}
