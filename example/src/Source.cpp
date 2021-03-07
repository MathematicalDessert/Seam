#include <iostream>
#include <parse/Parser.h>
#include <core/Module.h>
#include <core/Debug.h>

int main(int argc, char *argv[]) {
	const auto mod = std::make_unique<seam::Module>("test", std::string{ argv[1] });
	const auto parser = new seam::Parser(mod.get());

	seam::Debug::output_module_tokens(mod.get());

	try {
		parser->parse();
	} catch (seam::LexicalException& e) {
		std::cout << e.get_position().line << ":" << e.get_position().column << ":";
		std::cout << e.what() << std::endl;
	}

	return 0;
}
