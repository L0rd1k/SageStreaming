#pragma once 

#include <chrono>

#include "WindowPainterBase.h"
#include "image/PicturePainter.h"

class WindowPainterGlut : public WindowPainterBase {
public:
    WindowPainterGlut();
    virtual ~WindowPainterGlut();
    bool createWindow(int argc, char** argv, um::Size<int> size) override;
    void run() override; 
    static WindowPainterGlut& inst();
    static void displayEvent();
    static void idleEvent();
    static void reshapeEvent(int width, int height);
    static um::Size<int> _winSize;
private:
    static std::chrono::system_clock::time_point prevTime;
}; 