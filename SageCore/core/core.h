#pragma once

#include "definitions/local_definitions.h"
#include "image/picture_painter.h"
#include "substance/substance.h"
#include "config/ini_parser.h"

#ifdef USE_GLUT
#include "window/window_painter_glut.h"
#elif USE_GLFW
#include "window/window_painter_glfw.h"
#endif

#ifdef USE_IMGUI
#include "layer/imgui_layer.h"
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
    void createSingleSubstance(const sage::CameraState& camState);

    void createWindow(int argc, char** argv);
    void connectCamBufToWindow();
    void enableWindow();
    void enableCallbacks();
#ifdef USE_IMGUI
    void sendLog(const std::string& str);
#endif
public:
    SubstanceList _substns;
    sage::Scope<WindowPainterBase> _window;
    sage::Ref<PicturePainter> _pic;
private:
    std::vector<sage::Scope<void*>> global_callbacks;
};

}  // namespace sage