#include <chrono>
#include <iostream>

#include "callbacks/Signal.h"
#include"cameras/CamerasCreator.h"

void testFunction(int val) {
	std::cout << val << std::endl;
}

void writeText(std::string text, int val) {
	std::cout << text << " " << val << std::endl;
}

void cameraStream() {
	CamerasCreator creator;
	CamerasHandler* camFFmpeg = creator.createCamera(cam::CamTypes::FFMPEG);
	if(camFFmpeg) {
		Log() << "True";
		camFFmpeg->start();
	} else {
		Log() << "False";
	}

	while(true) {
	}
	delete camFFmpeg;
}

int main(int argc, char**argv) {
	cameraStream();


	Signal<int> _myEvent;
	_myEvent.connect(&testFunction);
	for(int i = 0; i < 100000; i++) {
		_myEvent.emit(std::move(i));
	}
	return 0;
}
