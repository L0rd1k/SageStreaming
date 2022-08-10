#pragma once

#include <thread>

// #include "utils/gl_header.h"
#ifdef USE_GLUT
#include <GL/gl.h>
#elif USE_GLFW
#include <glad/glad.h>
#endif


#include "image/image.h"
#include "image/image_queue.h"
#include "utils/gl_definitions.h"
#include "utils/log.h"
#include "utils/size.h"

class Texture {
public:
    Texture(GLuint id);
    virtual ~Texture();
    void setId(GLuint id);
    void draw(int x, int y, sage::Size<int> sz);
    void initBuffer(const ImageQueue* data);
    bool getLastDataFromQueue();

private:
    GLuint _id;
    const ImageQueue* _queueData;

    int64_t _lastTime;
    sage::Size<int> _lastSize;
    GLenum _lastFormat;
};