#pragma once

#include <atomic>
#include <vector>
#include <memory>

#ifdef __linux__ 
    #include <GL/gl.h>
    #include <GL/glut.h>
#endif


#include "texture/Texture.h"
#include "definitions/LocalDefinitions.h"

class PicturePainter {
public:
    PicturePainter(uint8_t textCount = gl::txtCount);
    virtual ~PicturePainter();
    void initTextures();
    void show();
    void allocateTextures();

    std::atomic<bool> _isInited;
private:
    std::vector<std::shared_ptr<Texture> > _textures;
};