#pragma once 

#ifdef __linux__ 
    #include <GL/gl.h>
    #include <GL/glut.h>
#endif

#include <unistd.h>
#include <sys/time.h>

#include "utils/Log.h"

class WindowPainterBase {
public:
    WindowPainterBase();
    virtual ~WindowPainterBase();

    static WindowPainterBase& inst();

    virtual bool createWindow(int argc, char** argv);
    virtual void run();
};
