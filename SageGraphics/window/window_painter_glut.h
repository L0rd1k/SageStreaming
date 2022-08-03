#pragma once 

#include <chrono>
#include <mutex>

#include "window_painter_base.h"
#include "image/picture_painter.h"

class WindowPainterGlut : public WindowPainterBase {
public:
    WindowPainterGlut();
    virtual ~WindowPainterGlut();
    bool createWindow(int argc, char** argv, sage::Size<int> size) override;
    void run() override; 
    static WindowPainterGlut& inst();
    static void displayEvent();
    static void idleEvent();
    static void reshapeEvent(int width, int height);
    static sage::Size<int> _winSize;
private:
    static std::chrono::system_clock::time_point prevTime;
    std::mutex _mtx;
}; 