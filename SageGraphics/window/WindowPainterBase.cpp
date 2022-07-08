#include "WindowPainterBase.h"

std::shared_ptr<PicturePainter> WindowPainterBase::_painter;


WindowPainterBase::WindowPainterBase() {
}

WindowPainterBase::~WindowPainterBase() {
}

bool WindowPainterBase::createWindow(int argc, char** argv) {
}

void WindowPainterBase::setPicturePainter(std::shared_ptr<PicturePainter> ptr) {
    _painter = ptr;
}

std::shared_ptr<PicturePainter> WindowPainterBase::getPicturePainter() {
    return _painter;    
}

void WindowPainterBase::run() {
}

WindowPainterBase& WindowPainterBase::inst() {
    static WindowPainterBase inst;
    return inst;
}

