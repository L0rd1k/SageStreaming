#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#define IMGUI_IMPL_API
#include <mutex>
#include <queue>
#include <string>
#include <unordered_map>

#include "callbacks/signal.h"
#include "definitions/local_definitions.h"
#include "utils/elapsed_timer.h"

#define IMGUI_IMPL_API
#include "imgui_console.h"

namespace sage {

class GuiLayer {
public:
    void init();
    void detach();
    void beginDraw();
    void endDraw();
    void processDraw();
    void setGuiWindow(GLFWwindow* win);
    void appendLog(const std::string& str);
    void appendSubstInfo(const SubstanceState& subst);
    void appendSubstState(const CameraState& subst);
    static bool first_time;

private:
    void dockMenuBar();
    void dockSettings();
    void dockLog();
    void dockViewport();
    void winManager();

private:
    GLFWwindow* _winPtr = NULL;
    float x = 0.0f, y = 0.0f;
    ImGuiID dockspace_id;
    static bool _winManager;
    static ImGuiLog log;

    struct PlottingSubstInfo {
        float _fpsValues[25] = {};
        int _valOffset = 0;
        ElapsedTimer _timerFps;
    };

    std::vector<PlottingSubstInfo> _plotInfo;
    std::unordered_map<uint8_t, const SubstanceState*> substanceInfo;
    std::unordered_map<uint8_t, const CameraState*> substanceState;

    std::mutex _mtx;
};

}  // namespace sage
