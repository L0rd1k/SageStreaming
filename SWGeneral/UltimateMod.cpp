#include <chrono>
#include <iostream>
#include <vector>

#include "substance/UMSubstance.h"
#include "window/WindowPainterGlut.h"
#include "callbacks/Signal.h"
#include "image/PicturePainter.h"

std::vector<std::shared_ptr<UMSubstance> > substancesList;

int testFunction(int a) {
	return a;
}

int main(int argc, char**argv) {
	for(int id = 0; id < 2; id++) {
		substancesList.push_back(std::make_shared<UMSubstance>(id));
		substancesList.back()->enableSubstance();
	}

	std::unique_ptr<WindowPainterBase> window = std::make_unique<WindowPainterGlut>();
	auto pic = std::make_shared<PicturePainter>(2);
	window->setPicturePainter(pic);

	for(int id = 0; id < 2; id++) {
		if(substancesList[id]->getCamera()) {
			// const auto &imgQueue = substancesList[id]->getCamera()->getQueue();
			pic->setDataBuffer(id, substancesList[id]->getImageQueue());
		}
	}

	window->createWindow(argc, argv);
	window->run();
	// Signal<int> _myEvent;
	// _myEvent.connect(&testFunction);
	// for(int i = 0; i < 100000; i++) {
	// 	_myEvent.emit(std::move(i));
	// }
	return 0;
}
