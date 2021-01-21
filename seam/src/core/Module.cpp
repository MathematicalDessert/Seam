#include <fstream>

#include "core/Module.h"
#include "core/Exception.h"

namespace seam {
	void Module::load_source() {
		std::ifstream source_stream(file_path_);

		if (!source_stream) {
			throw SourceLoadException("failed to open source module file at path: " + file_path_);
		}

		source_ = std::string(std::istreambuf_iterator<char>(source_stream),
								std::istreambuf_iterator<char>());
	}

	Module::Module(std::string module_name, std::string file_path)
		: module_name_(std::move(module_name)), file_path_(std::move(file_path)) {
		load_source();
	}

	const std::string& Module::get_source() const noexcept {
		return source_;
	}

	LexState Module::get_lex_state() {
		return LexState(source_);
	}
}
