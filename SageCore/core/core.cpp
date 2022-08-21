#include "core.h"

void sage::Core::createSubstances(int count = 0) {
    for (int id = 0; id < count; id++) {
        _substns.push_back(std::make_shared<sage::Substance>(id));
        _substns.back()->enableSubstance();
    }
}

void sage::Core::createWindow(int argc, char**argv) {
#ifdef USE_GLUT
    _window = std::make_unique<WindowPainterGlut>();
#elif USE_GLFW
    _window = std::make_unique<WindowPainterGLFW>();
#endif
    _window->createWindow(argc, argv, sage::Size<int>(1024, 768));
    _pic = std::make_shared<PicturePainter>(_substns.size());
    _window->setPicturePainter(_pic);
}

void sage::Core::enableWindow() {
    _window->run();
}

void sage::Core::connectCamBufToWindow() {
    for (int id = 0; id < _substns.size(); id++) {
        if (_substns[id]->_decoder) {
            _pic->setDataBuffer(id, _substns[id]->_decoder->getQueue());
        }
    }
}
