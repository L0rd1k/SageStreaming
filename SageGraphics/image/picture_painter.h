#pragma once

#include <atomic>
#include <vector>
#include <memory>
#include <numeric>

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
    void reinitTextures();

    void retrieveTextureSize(sage::Size<int> &size);

    void allocateTextures(size_t textSize);
    void createTexture();
    void clearTextures();
    void popTexture(uint textureId);

    void setDataBuffer(uint8_t textId, const ImageQueue* buffer);
    void show(const sage::Size<int>& winSize, sage::Size<int>& textSize);
    uint8_t getTexturesCount();

    std::vector<std::shared_ptr<Texture>> &getTextures();
public:
    std::atomic<bool> _isInited;
    std::mutex _mtx;
private:
    std::vector<std::shared_ptr<Texture> > _textures;
    std::vector<uint> textures;
    
};