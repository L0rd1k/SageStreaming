#pragma once

#include "config/ini_parser.h"
#include "config/config_params.h"

#include "definitions/local_definitions.h"
#include "image/picture_painter.h"
#include "substance/substance.h"

#include <unordered_map>
#include <mutex>
#include <utility>
#include <vector>
#include <map>

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
    using SubstanceMap = std::map<uint8_t, sage::Ref<sage::Substance>>;

    // using SubstanceVec = std::vector<sage::Scope<sage::Substance>>;

    /** @brief Singleton for Core class.
     * @return Reference to Core class. **/  
    static Core& inst() {
        static Core inst;
        return inst;
    }
    virtual ~Core() = default;
    
    /** @brief Create substances from config. **/
    void createSubstances();

    void createSingleSubstance(const sage::SubstanceState& camState);

    void removeSingleSubstance(const uint8_t& id);

    /** @brief Run created substances. **/
    void runSubstances();

    void stopSubstances();


    /** @brief Create gui window.
     * @param [in] argc Number of parameters.
     * @param [in] argc Array of pointers to param's string. **/ 
    void createWindow(int argc, char** argv);

    /** @brief Connect texture with substance img queue. **/ 
    void connectCamBufToWindow();
    void enableWindow();
    void enableCallbacks();
#ifdef USE_IMGUI
    void sendLog(const std::string& str, const sage::LogLevels& lvl);
#endif
public:
    // SubstanceVec _substns;  //> Vector of substances.
    SubstanceMap _activeSubstns;
    SubstanceMap _unusedSubstns;

    sage::Scope<WindowPainterBase> _window; //> Type of window painter
    sage::Ref<PicturePainter> _pic; 
private:
    std::multimap<uint8_t, sage::Scope<void*>> substances_callbacks;
    std::vector<sage::Scope<void*>> global_callbacks;

    std::mutex mtx_;
};

}  // namespace sage