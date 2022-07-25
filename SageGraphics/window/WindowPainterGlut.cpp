#include "WindowPainterGlut.h"

std::chrono::system_clock::time_point WindowPainterGlut::prevTime;
um::Size<int> WindowPainterGlut::_winSize;


WindowPainterGlut::WindowPainterGlut() :
WindowPainterBase() {
}

WindowPainterGlut::~WindowPainterGlut() {
}


bool WindowPainterGlut::createWindow(int argc, char** argv, um::Size<int> size) {
    _winSize = size;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(size.width(), size.height());
    if(glutCreateWindow("UlimadeMode") <= 0) {
        Log() << "[Error] Create window <glut>";
        return false;
    }
    glutDisplayFunc(displayEvent);
    glutReshapeFunc(reshapeEvent);
    glutIdleFunc(idleEvent);
    return true;
}

void WindowPainterGlut::run() {
    glutMainLoop();
}

void WindowPainterGlut::displayEvent() {
    if(_painter) {
        if(!_painter->_isInited) {
            _painter->initTextures();
            _painter->_isInited = true;
        }
        _painter->show();
    }
}

void WindowPainterGlut::reshapeEvent(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, _winSize.width(), _winSize.height(), 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void WindowPainterGlut::idleEvent() {
    // std::lock_guard<std::mutex> _lock(inst()._mtx);
    prevTime = {};
    static std::chrono::system_clock::time_point currTime = std::chrono::high_resolution_clock::now(); 
    double elapsed = std::chrono::duration<double, std::milli>(currTime - prevTime).count();
    if((elapsed / 1000) > 0 || elapsed > 1000 / 25) {
        prevTime = currTime;
        glutPostRedisplay();
    }
    usleep(1000);
}

WindowPainterGlut& WindowPainterGlut::inst() {
    static WindowPainterGlut inst;
    return inst;
}