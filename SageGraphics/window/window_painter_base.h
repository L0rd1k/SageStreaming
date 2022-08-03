#pragma once 

#ifdef __linux__ 
    #include <GL/gl.h>
    #include <GL/glut.h>
#endif

#include <unistd.h>
#include <memory>
#include <sys/time.h>

#include "utils/log.h"
#include "image/picture_painter.h"

class WindowPainterBase {
public:
    WindowPainterBase();
    virtual ~WindowPainterBase();

    void setPicturePainter(std::shared_ptr<PicturePainter> ptr);
    std::shared_ptr<PicturePainter> getPicturePainter();

    virtual bool createWindow(int argc, char** argv, sage::Size<int> _size) = 0;
    virtual void run() = 0;
    
protected:
    static std::shared_ptr<PicturePainter> _painter;
private:
    std::mutex _mutex;
};