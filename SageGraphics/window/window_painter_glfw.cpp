#include "window_painter_glfw.h"

sage::Size<int> WindowPainterGLFW::_winSize;
sage::Size<int> WindowPainterGLFW::_textSize;


WindowPainterGLFW::WindowPainterGLFW() : WindowPainterBase() {
#ifdef USE_IMGUI
    _gui = new sage::GuiLayer();
#endif
}

WindowPainterGLFW::~WindowPainterGLFW() {
#ifdef USE_IMGUI
    _gui->detach();
    delete _gui;
#endif
}

WindowPainterGLFW& WindowPainterGLFW::inst() {
    static WindowPainterGLFW inst;
    return inst;
}

GLFWwindow* WindowPainterGLFW::getWindow() {
    return _window;
}

#ifdef USE_IMGUI
sage::GuiLayer* WindowPainterGLFW::getGuiLayer() {
    return _gui;
}
#endif

void WindowPainterGLFW::extractMonitorSize(uint32_t& height, uint32_t& width) {
    int count;
    GLFWmonitor** monitors = glfwGetMonitors(&count);
    std::cout << "Monitor count:" << count << std::endl;
    for(uint8_t i = 0; i < count; i++) {
        GLFWmonitor* mon = monitors[i];
        const GLFWvidmode* mode = glfwGetVideoMode(mon);
        if(mode->width > width) {
            width = mode->width;
            height = mode->height;
        }
    }
}

bool WindowPainterGLFW::createWindow(int argc, char** argv, sage::Size<int> size) {
    if (!_isInited) {
        if (!glfwInit()) {
            Log::error("GLFW initialization error.");
        }
        extractMonitorSize(_data.height, _data.width);
        _isInited = true;
    }

    _winSize = sage::Size<int>(_data.width, _data.height);
    _window = glfwCreateWindow(_data.width, _data.height, "SageStreaming", nullptr, nullptr);
    // _window = glfwCreateWindow(size.width(), size.height(), "SageStreaming", nullptr, nullptr);
    /** Fullscreen mode. **/
    // _window = glfwCreateWindow(size.width(), size.height(),"SageStreaming", glfwGetPrimaryMonitor(), nullptr);
    glfwMakeContextCurrent(_window);
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);  //> Make use of existing GLFW loader;
    Log::info("OpenGL renderer:", glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_VERSION));
#ifdef USE_IMGUI
    _gui->setGuiWindow(_window);
#endif
    glfwSetWindowUserPointer(_window, &_data);
    glfwSetWindowSizeCallback(_window, reshapeEvent);
    setVSync(true);
}

void WindowPainterGLFW::reshapeEvent(GLFWwindow* window, int width, int height) {
    _winSize.setHeight(height);
    _winSize.setWidth(width);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    _painter->retrieveTextureSize(_textSize);

    // double ratioX = width / (float)textureWidth;
    // double ratioY = height / (float)textureHeight;
    // double ratio = (ratioX < ratioY) ? ratioX : ratioY;
    // double viewWidth = textureWidth * ratio;
    // double viewHeight = textureHeight * ratio;
    // double viewX = (width - textureWidth * ratio) / 2;
    // double viewY = (height - textureHeight * ratio) / 2;
    // glViewport(viewX, viewY, viewWidth, viewHeight);
    // glMatrixMode(GL_PROJECTION);
    // glLoadIdentity();
    // _winSize.setWidth(width);
    // _winSize.setHeight(height);
    // glOrtho(0, width, height, 0, -1, 1);
    // glMatrixMode(GL_MODELVIEW);
    // glLoadIdentity();
}

void WindowPainterGLFW::run() {
    /** @todo Find a better way of intital window shaping. **/
#ifdef USE_IMGUI
    std::cout << "Init" << std::endl;
    _gui->init();
#endif
    reshapeEvent(_window, _data.width, _data.height);
    static uint8_t counter;
    while (!glfwWindowShouldClose(_window)) {
        if (_painter) {
            if (!_painter->_isInited) {
                _painter->initTextures();
                _painter->_isInited = true;
            }
#ifdef USE_IMGUI
            _gui->beginDraw();
            _gui->processDraw();
#endif      
            _painter->show(_winSize, _textSize);
#ifdef USE_IMGUI
            _gui->endDraw();
#endif
            glfwPollEvents();
            glfwSwapBuffers(_window);
            if(_hzTimer.elapsed() > 1) {
                std::cout << "Hz glfw frame: " << (int)counter << std::endl;
                counter = 0; 
                _hzTimer.restart();
            } else {
                counter++;
            }
        }
    }
}

void WindowPainterGLFW::close() {
    glfwDestroyWindow(_window);
}

void WindowPainterGLFW::setVSync(bool enable) {
    enable ? glfwSwapInterval(1) : glfwSwapInterval(0);
}
