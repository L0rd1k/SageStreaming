#pragma once

#include <chrono>
#include <mutex>

// #include "utils/gl_header.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "image/picture_painter.h"
#include "window_painter_base.h"

class WindowPainterGLFW : public WindowPainterBase {
public:
    WindowPainterGLFW();
    virtual ~WindowPainterGLFW();
    bool createWindow(int argc, char** argv, sage::Size<int> size) override;
    void run() override;
    void close();
    static WindowPainterGLFW& inst();
    static sage::Size<int> _winSize;
private:
    void setVSync(bool enable);
private:
    GLFWwindow* _window;
    static void reshapeEvent(GLFWwindow* window, int width, int height);
    struct WindowData {
        unsigned int width, height;
    };
    WindowData _data;
};