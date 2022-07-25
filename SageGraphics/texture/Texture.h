#pragma once

#ifdef __linux__
#include <GL/gl.h>
#include <GL/glut.h>
#endif

#include <thread>

#include "image/Image.h"
#include "image/ImageQueue.h"
#include "utils/GLDefinitions.h"
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
    const ImageQueue* _queueData;

    int64_t _lastTime;
    um::Size<int> _lastSize;
    GLenum _lastFormat;
};