#include <chrono>

#include "Patterns/Patterns.h"
#include "Threads/Thread.h"
#include "callbacks/Signal.h"
#include "ffmpeg/CameraFFmpeg.h"

void testFunction(int val) {
	std::cout << val << std::endl;
}

void writeText(std::string text, int val) {
	std::cout << text << " " << val << std::endl;
}

int main(int argc, char**argv) {
	Signal<int> _myEvent;
	_myEvent.connect(&testFunction);
	for(int i = 0; i < 100000; i++) {
		_myEvent.emit(std::move(i));
	}
	return 0;
}
