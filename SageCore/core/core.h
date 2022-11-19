#pragma once

#include "config/ini_parser.h"
#include "definitions/local_definitions.h"
#include "image/picture_painter.h"
#include "substance/substance.h"

#include <unordered_map>
#include <mutex>

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
    using SubstanceMap = std::unordered_map<uint8_t, sage::Ref<sage::Substance>>;
    /** @brief Singleton for Core class.
     * @return Reference to Core class. **/  
    static Core& inst() {
        static Core inst;
        return inst;
    }
    virtual ~Core() = default;
    /** @brief Create substances from config.
     * @param [in] count Count of contexs **/
    void createSubstances(uint8_t count);
    void createSingleSubstance(const sage::CameraState& camState);
    /** @brief Run created substances. **/
    void runSubstances();
    /** @brief Create gui window.
     * @param [in] argc Number of parameters.
     * @param [in] argc Array of pointers to param's string. **/ 
    void createWindow(int argc, char** argv);
    /** @brief Connect texture with substance img queue. **/ 
    void connectCamBufToWindow();
    void enableWindow();
    void enableCallbacks();
#ifdef USE_IMGUI
    void sendLog(const std::string& str);
#endif
public:
    SubstanceMap _substns;  //> Map of substances.
    sage::Scope<WindowPainterBase> _window; //> Type of window painter
    sage::Ref<PicturePainter> _pic; 
private:
    std::vector<sage::Scope<void*>> global_callbacks;
};

}  // namespace sage