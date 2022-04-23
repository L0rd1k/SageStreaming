#include "WindowPainterGlut.h"

std::chrono::system_clock::time_point WindowPainterGlut::prevTime;

WindowPainterGlut::WindowPainterGlut() :
WindowPainterBase() {
    _painter = std::make_unique<PicturePainter>(gl::txtCount);
}

WindowPainterGlut::~WindowPainterGlut() {
}

bool WindowPainterGlut::createWindow(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(800, 600);
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
    if(inst()._painter) {
        if(!inst()._painter->_isInited) {
            inst()._painter->initTextures();
            inst()._painter->_isInited = true;
        }
        inst()._painter->show();
    }
}

void WindowPainterGlut::reshapeEvent(int width, int height) {
}

void WindowPainterGlut::idleEvent() {
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