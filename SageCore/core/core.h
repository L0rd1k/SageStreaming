#pragma once

#include "definitions/sage_definitions.h"
#include "image/picture_painter.h"
#include "substance/substance.h"

#ifdef USE_GLUT
    #include "window/window_painter_glut.h"
#elif USE_GLFW
    #include "window/window_painter_glfw.h"
#endif

namespace sage {
class Core {
public:
    using SubstanceList = std::vector<sage::Ref<sage::Substance>>;

    static Core& inst() {
        static Core inst;
        return inst;
    }
    virtual ~Core() = default;
    void createSubstances(int count);
    void createWindow(int argc, char** argv);
    void connectCamBufToWindow();
    void enableWindow();

private:
    SubstanceList _substns;
    sage::Scope<WindowPainterBase> _window;
    sage::Ref<PicturePainter> _pic;
};

}  // namespace sage