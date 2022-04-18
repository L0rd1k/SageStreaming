#include "WindowPainterGlut.h"

WindowPainterGlut::WindowPainterGlut() :
WindowPainterBase() {
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
    return true;
}

void WindowPainterGlut::displayEvent() {
           
}