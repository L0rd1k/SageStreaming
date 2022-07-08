#pragma once

#ifdef __linux__ 
    #include <GL/gl.h>
    #include <GL/glut.h>
#endif

#include "image/ImageQueue.h"
#include "image/Image.h"
#include "utils/Log.h"
#include "utils/Size.h"

class Texture {
public:
    Texture(GLuint id);
    virtual ~Texture();
    void setId(GLuint id);
    void draw(int x, int y, um::Size<int> sz);
    void initBuffer(const ImageQueue* data);
    bool getLastDataFromQueue();
private:
    GLuint _id;
    const ImageQueue *_queueData;
};