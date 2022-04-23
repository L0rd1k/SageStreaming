#pragma once

#ifdef __linux__ 
    #include <GL/gl.h>
    #include <GL/glut.h>
#endif

#include "utils/Log.h"

class Texture {
public:
    Texture(GLuint id);
    virtual ~Texture();
    void setId(GLuint id);
private:
    GLuint _id;
};