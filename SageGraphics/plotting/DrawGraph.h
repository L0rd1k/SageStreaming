#pragma once

#include <iostream>

#ifdef __linux__ 
    #include <GL/glut.h>
#endif

class DrawGraph {
public:
    DrawGraph();
    virtual ~DrawGraph();
    static int createWindow(int argc, char**argv);
private:
    static void display();
};