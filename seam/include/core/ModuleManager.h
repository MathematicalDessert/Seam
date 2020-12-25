#pragma once

#include <memory>
#include <unordered_map>

#include "Module.h"

namespace seam {
	class ModuleManager {
		//std::unordered_map<std::string, std::unique_ptr<Module>> modules_;
	public:
		void add_module(std::string module_name, std::string file_path);
	};
}
