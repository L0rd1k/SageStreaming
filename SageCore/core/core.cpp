#include "core.h"

void sage::Core::createSubstances(int count = 0) {
    for (int id = 0; id < count; id++) {
        _substns.push_back(std::make_shared<sage::Substance>(id));
        _substns.back()->enableSubstance();
    }
}

void sage::Core::createWindow(int argc, char**argv) {
    _window = std::make_unique<WindowPainterGlut>();
    _pic = std::make_shared<PicturePainter>(1);
    _window->setPicturePainter(_pic);
    _window->createWindow(argc, argv);
}

void sage::Core::enableWindow() {
    _window->run();
}

void sage::Core::connectCamBufToWindow() {
    for (int id = 0; id < _substns.size(); id++) {
        if (_substns[id]->getCamera()) {
            // const auto &imgQueue = substancesList[id]->getCamera()->getQueue();
            _pic->setDataBuffer(id, _substns[id]->getImageQueue());
        }
    }
}
