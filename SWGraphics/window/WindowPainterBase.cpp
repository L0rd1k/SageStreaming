#include "WindowPainterBase.h"

WindowPainterBase::WindowPainterBase() {
}

WindowPainterBase::~WindowPainterBase() {
}

bool WindowPainterBase::createWindow(int argc, char** argv) {
}

void WindowPainterBase::run() {
}

WindowPainterBase& WindowPainterBase::inst() {
    static WindowPainterBase inst;
    return inst;
}

