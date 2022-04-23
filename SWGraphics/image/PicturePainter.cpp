#include "PicturePainter.h"


PicturePainter::PicturePainter(uint8_t textCount) :
_isInited(false), 
_textures(textCount, nullptr) {
    allocateTextures();
}

PicturePainter::~PicturePainter() {
}

void PicturePainter::allocateTextures() {
    for(auto itr = 0; itr < _textures.size(); itr++) {
        _textures[itr] = std::make_shared<Texture>(-1);
    }
}

void PicturePainter::show() {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers();
}

void PicturePainter::initTextures() {
    GLuint textures[gl::txtCount];
    glEnable(GL_TEXTURE_2D);
    glGenTextures(gl::txtCount, textures);

    for(int itr = 0; itr < gl::txtCount; itr++) {
        Log() << itr;
        int textId = textures[itr];
        glBindTexture(GL_TEXTURE_2D, textId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
        _textures[itr]->setId(textId);
    }
}