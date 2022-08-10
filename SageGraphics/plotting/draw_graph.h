#pragma once

#include <iostream>

#ifdef USE_GLUT
#include <GL/gl.h>
#elif USE_GLFW
#include <glad/glad.h>
#endif

// #include "utils/gl_header.h"

class DrawGraph {
public:
    DrawGraph();
    virtual ~DrawGraph();
    static int createWindow(int argc, char**argv);
private:
    static void display();
};