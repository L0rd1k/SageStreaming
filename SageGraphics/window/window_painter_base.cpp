#include "window_painter_base.h"

std::shared_ptr<PicturePainter> WindowPainterBase::_painter;

WindowPainterBase::WindowPainterBase() {
}

WindowPainterBase::~WindowPainterBase() {
}

void WindowPainterBase::setPicturePainter(std::shared_ptr<PicturePainter> ptr) {
    std::lock_guard<std::mutex> _lock(_mutex);
    _painter = ptr;
}

std::shared_ptr<PicturePainter> WindowPainterBase::getPicturePainter() {
    std::lock_guard<std::mutex> _lock(_mutex);
    return _painter;    
}

#ifdef USE_IMGUI
    sage::GuiLayer* WindowPainterBase::getGuiLayer() {
        return nullptr;
    }
#endif