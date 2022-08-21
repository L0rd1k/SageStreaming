#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <chrono>
#include <mutex>

#include "image/picture_painter.h"
#include "window_painter_base.h"

#ifdef USE_IMGUI
#include "layer/imgui_layer.h"
#endif

class WindowPainterGLFW : public WindowPainterBase {
public:
    WindowPainterGLFW();
    virtual ~WindowPainterGLFW();
    bool createWindow(int argc, char** argv, sage::Size<int> size) override;
    void run() override;
    void close();
    static WindowPainterGLFW& inst();
    static sage::Size<int> _winSize;
    GLFWwindow* getWindow();
private:
    void setVSync(bool enable);
private:
#ifdef USE_IMGUI
    sage::GuiLayer* _gui = nullptr;
#endif
    GLFWwindow* _window;
    static void reshapeEvent(GLFWwindow* window, int width, int height);
    struct WindowData {
        unsigned int width, height;
    };
    WindowData _data;
};