#pragma once

#include <stdexcept>

#include "position.hpp"

namespace seam::core::utils
{
	struct lexical_exception final : std::runtime_error
	{
		position position;
		
		explicit lexical_exception(const utils::position position, const std::string& msg) :
			std::runtime_error(msg), position(position)
		{}

		explicit lexical_exception(const std::size_t line, const std::size_t column, const std::string& msg) :
			std::runtime_error(msg), position({ line, column })
		{}
	};

	struct parser_exception final : std::runtime_error
	{
		position position;

		explicit parser_exception(const utils::position position, const std::string& msg) :
			std::runtime_error(msg), position(position)
		{}

		explicit parser_exception(const std::size_t line, const std::size_t column, const std::string& msg) :
			std::runtime_error(msg), position({ line, column })
		{}
	};


	struct compiler_exception final : std::runtime_error
	{
		position position;

		explicit compiler_exception(const utils::position position, const std::string& msg) :
			std::runtime_error(msg), position(position)
		{}

		explicit compiler_exception(const std::size_t line, const std::size_t column, const std::string& msg) :
			std::runtime_error(msg), position({ line, column })
		{}
	};

	struct type_exception final :std::runtime_error
	{
		position position;

		explicit type_exception(const utils::position position, const std::string& msg) :
			std::runtime_error(msg), position(position)
		{}

		explicit type_exception(const std::size_t line, const std::size_t column, const std::string& msg) :
			std::runtime_error(msg), position({ line, column })
		{}
	};
}
