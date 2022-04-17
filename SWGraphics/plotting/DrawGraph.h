#pragma once

#include <iostream>

#ifdef __linux__ 
    #include <GL/glut.h>
    #include <GLFW/glfw3.h>
#endif

class DrawGraph {
public:
    DrawGraph();
    virtual ~DrawGraph();
    static int createWindow(int argc, char**argv);
private:
    static void display();
};