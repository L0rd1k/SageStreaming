#pragma once

#include <atomic>
#include <vector>
#include <memory>

#ifdef USE_GLUT
#include <GL/gl.h>
#elif USE_GLFW
#include <glad/glad.h>
#endif

#include "texture/texture.h"
#include "definitions/local_definitions.h"
#include "image/image_queue.h"
#include "utils/size.h"

class PicturePainter {
public:
    PicturePainter(uint8_t textCount = sage::txtCount);
    virtual ~PicturePainter();
    void initTextures();
    void allocateTextures();
    void setDataBuffer(uint8_t textId, const ImageQueue* buffer);
    void show();
public:
    std::atomic<bool> _isInited;
private:
    std::vector<std::shared_ptr<Texture> > _textures;
};