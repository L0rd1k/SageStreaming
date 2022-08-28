#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#define IMGUI_IMPL_API
#include "imgui_console.h"
#include "definitions/sage_definitions.h"

#include <string>
#include <mutex>
#include <unordered_map>

namespace sage {

class GuiLayer {
public:
    void init();
    void detach();
    void beginDraw();
    void endDraw();
    void processDraw();
    void setGuiWindow(GLFWwindow* win);
    void appendLog(const std::string &str);
    void appendSubstInfo(const SubstanceInfo& subst);
private:
    void dockMenuBar();
    void dockSettings();
    void dockLog();
    void dockViewport();
    void winManager();
    GLFWwindow* _winPtr = NULL;
    float x = 0.0f, y = 0.0f;

    ImGuiID dockspace_id;
    static bool _winManager;
    static ImGuiLog log;
    std::unordered_map<uint8_t, const SubstanceInfo*> substanceInfo;

    std::mutex _mtx;
};

}  // namespace sage
