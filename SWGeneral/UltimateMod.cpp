#include "Patterns/Patterns.h"
#include "Threads/Thread.h"
#include <chrono>

#include "callbacks/Signal.h"

void testFunction(int val) {
	std::cout << val << std::endl;
}

int main(int argc, char**argv) {
	Signal<int> _eventMy;
	_eventMy.add(&testFunction);
	_eventMy.emit(5);

	return 0;
}
