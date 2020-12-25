#include <iostream>
#include <parse/LexState.h>

int main()
{
	auto source = seam::LexState("ABCDE\nFAAABO");
	std::cout << source.peek_character() << std::endl;
	for (auto i = 0; i < 5; i++) {
		source.next_character();
	}
	
	source.discard();
	source.skip_character();
	for (auto i = 0; i < 5; i++) {
		source.next_character();
	}
	std::cout << source.get_line(8) << std::endl;
	
	std::cout << source.consume() << std::endl;
	std::cout << source.peek_character() << std::endl;
	source.next_character();
	std::cout << source.consume() << std::endl;
	std::cout << source.peek_character() << std::endl;
	return 0;
}