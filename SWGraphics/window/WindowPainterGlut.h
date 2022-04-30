#pragma once 

#include <chrono>

#include "WindowPainterBase.h"
#include "image/PicturePainter.h"

class WindowPainterGlut : public WindowPainterBase {
public:
    WindowPainterGlut();
    virtual ~WindowPainterGlut();
    bool createWindow(int argc, char** argv) override;
    void run() override; 
    static WindowPainterGlut& inst();
    static void displayEvent();
    static void idleEvent();
    static void reshapeEvent(int width, int height);
private:
    static std::chrono::system_clock::time_point prevTime;
}; 