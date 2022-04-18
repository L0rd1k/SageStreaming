#pragma once 

#ifdef __linux__ 
    #include <GL/gl.h>
    #include <GL/glut.h>
#endif

#include "utils/Log.h"

class WindowPainterBase {
public:
    WindowPainterBase();
    virtual ~WindowPainterBase();
    virtual bool createWindow(int argc, char** argv) = 0;
};
