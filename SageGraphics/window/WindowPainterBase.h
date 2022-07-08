#pragma once 

#ifdef __linux__ 
    #include <GL/gl.h>
    #include <GL/glut.h>
#endif

#include <unistd.h>
#include <memory>
#include <sys/time.h>

#include "utils/Log.h"
#include "image/PicturePainter.h"

class WindowPainterBase {
public:
    WindowPainterBase();
    virtual ~WindowPainterBase();
    static WindowPainterBase& inst();

    void setPicturePainter(std::shared_ptr<PicturePainter> ptr);
    std::shared_ptr<PicturePainter> getPicturePainter();

    virtual bool createWindow(int argc, char** argv);
    virtual void run();
protected:
    static std::shared_ptr<PicturePainter> _painter;
};
