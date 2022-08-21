#include "picture_painter.h"

PicturePainter::PicturePainter(uint8_t textCount)
    : _isInited(false),
      _textures(textCount, nullptr) {
    allocateTextures();
}

PicturePainter::~PicturePainter() {
}

void PicturePainter::allocateTextures() {
    for (auto itr = 0; itr < _textures.size(); itr++) {
        _textures[itr] = std::make_shared<Texture>(-1);
    }
}

uint8_t PicturePainter::getTexturesSize() {
    return _textures.size();
}

void PicturePainter::show(sage::Size<int> size) {
    unsigned char pix[4] = {0, 0, 0, 255};

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    int x[4] = {0, size.width() / 2, 0, size.width() / 2};
    int y[4] = {size.height() / 2, size.height() / 2, 0, 0};

    for (auto i = 0; i < _textures.size(); i++) {
        if (_textures[i]->getLastDataFromQueue()) {
            // _textures[i]->draw(x[i], y[i], sage::Size<int>(size.width()/2, size.height()/2));
            _textures[i]->draw(0, 0, sage::Size<int>(size.width(), size.height()));
        }
    }
    glRasterPos3f(0, 1, 0);
    glDrawPixels(1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pix);
}

void PicturePainter::initTextures() {
    GLuint textures[_textures.size()];
    glEnable(GL_TEXTURE_2D);
    glGenTextures(_textures.size(), textures);

    for (int itr = 0; itr < _textures.size(); itr++) {
        int textId = textures[itr];
        glBindTexture(GL_TEXTURE_2D, textId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
        _textures[itr]->setId(textId);
    }
}

void PicturePainter::setDataBuffer(uint8_t textId, const ImageQueue *buffer) {
    if (textId < (int)_textures.size() && textId >= 0) {
        _textures[textId]->initBuffer(buffer);
    }
}