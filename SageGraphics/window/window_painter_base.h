#pragma once 

#include <unistd.h>
#include <memory>
#include <sys/time.h>

#include "utils/log.h"
#include "image/picture_painter.h"

#ifdef USE_IMGUI
#include "layer/imgui_layer.h"
#endif

class WindowPainterBase {
public:
    WindowPainterBase();
    virtual ~WindowPainterBase();

    void setPicturePainter(std::shared_ptr<PicturePainter> ptr);
    std::shared_ptr<PicturePainter> getPicturePainter();
    
    virtual bool createWindow(int argc, char** argv, sage::Size<int> _size) = 0;
#ifdef USE_IMGUI
    virtual sage::GuiLayer* getGuiLayer();
#endif
    virtual void run() = 0;
    
protected:
    static std::shared_ptr<PicturePainter> _painter;
    int _isInited = false;
private:
    std::mutex _mutex;
};
