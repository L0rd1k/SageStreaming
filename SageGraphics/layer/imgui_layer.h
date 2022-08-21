#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>

namespace sage {

class GuiLayer {
public:
    void init();
    void detach();
    void beginDraw();
    void endDraw();
    void processDraw();
    void setGuiWindow(GLFWwindow* win);

private:
    void dockMenuBar();
    void dockSettingsWindow();
    void dockLogWindow();
    void dockViewportWindows();

    GLFWwindow* _winPtr = NULL;
    float x = 0.0f, y = 0.0f;
};

}  // namespace sage
