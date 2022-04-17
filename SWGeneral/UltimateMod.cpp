#include <chrono>
#include <iostream>

#include "substance/UMSubstance.h"
#include "callbacks/Signal.h"

// void testFunction(int val) {
// 	std::cout << val << std::endl;
// }

// void writeText(std::string text, int val) {
// 	std::cout << text << " " << val << std::endl;
// }

int main(int argc, char**argv) {
	for(int id = 0; id < 1; id++) {
		UMSubstance subst(id);
		subst.enableSubstance();
	}

	// Signal<int> _myEvent;
	// _myEvent.connect(&testFunction);
	// for(int i = 0; i < 100000; i++) {
	// 	_myEvent.emit(std::move(i));
	// }
	return 0;
}
