#pragma once

#include "definitions/sage_definitions.h"

#include "substance/substance.h"
#include "image/PicturePainter.h"

#include "window/WindowPainterGlut.h"

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
    void createWindow(int argc, char**argv);
    void connectCamBufToWindow();
    void enableWindow();
private:
    SubstanceList _substns;
    sage::Scope<WindowPainterBase> _window;
    sage::Ref<PicturePainter> _pic;
};

}  // namespace sage