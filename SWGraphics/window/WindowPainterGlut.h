#pragma once 

#include "WindowPainterBase.h"

class WindowPainterGlut : public WindowPainterBase {
public:
    WindowPainterGlut();
    virtual ~WindowPainterGlut();
    bool createWindow(int argc, char** argv) override;
    static void displayEvent();
};