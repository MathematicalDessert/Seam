#pragma once

#include <string_view>

#include "parse/LexState.h"

namespace seam {
	/**
	 * Internal representation of a source file.
	 */
	class Module {
		// name of module
		const std::string module_name_;
		
		// path to module file
		const std::string file_path_;

		// source of module
		std::string source_;

		/**
		 * Loads data from source file into source buffer.
		 */
		void load_source();
	public:
		/**
		 * @throws SourceLoadException if error occurs whilst loading file source.
		 */
		Module(std::string module_name, std::string file_path);

		/**
		 * Returns reference to module source.
		 *
		 * @returns source of module file.
		 */
		[[nodiscard]] const std::string& get_source() const noexcept;

		/**
		 * 
		 */
		[[nodiscard]] LexState get_lex_state() const;
	};
}
