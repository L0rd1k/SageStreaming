#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#define IMGUI_IMPL_API
#include <mutex>
#include <queue>
#include <string>
#include <unordered_map>

#include "layer/imgui_main_handler.h"


#include "callbacks/signal.h"
#include "definitions/local_definitions.h"
#include "utils/elapsed_timer.h"

#define IMGUI_IMPL_API
#include "imgui_console.h"

namespace sage {

class GuiMainWindow {
public:
    void init();
    void detach();
    void beginDraw();
    void endDraw();
    void processDraw();
    void setGuiWindow(GLFWwindow* win);
    void appendLog(const std::string& str, const sage::LogLevels& lvl);
    void appendSubstState(const SubstanceState& subst);
    void initFontsTexture();
    ImgGuiMainHandler* getGuiHandler();

private:
    void dockMenuBar();
    void dockSettings();
    void updateLog();
    void dockViewport();
    void winManager();

private:
    ImgGuiMainHandler imguiHandler_;
private:
    GLFWwindow* _winPtr = NULL;
    ImGuiLog log;

    std::mutex _mtx;
};

}  // namespace sage
