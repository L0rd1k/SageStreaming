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

void PicturePainter::createTexture() {
    std::lock_guard<std::mutex> locker(_mtx);

    _textures.push_back(std::make_shared<Texture>(-1));

    uint textId = textures[_textures.size() - 1];
    _textures[_textures.size() - 1]->setId(textId);
    glBindTexture(GL_TEXTURE_2D, textId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

}

uint8_t PicturePainter::getTexturesCount() {
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
            // Log::trace("SHOW", i, _textures.size(), size.width(), size.height());
            // _textures[i]->draw(x[i], y[i], sage::Size<int>(size.width()/2, size.height()/2));
            _textures[i]->draw(0, 0, sage::Size<int>(size.width(), size.height()));
        }
    }
    glRasterPos3f(0, 1, 0);
    glDrawPixels(1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pix);
}

void PicturePainter::initTextures() {
    textures.assign(25, 0);
    std::iota(std::begin(textures), std::end(textures), 1);
    glEnable(GL_TEXTURE_2D);
    glGenTextures(25, textures.data());
    for (int itr = 0; itr < _textures.size(); itr++) {
        uint textId = textures[itr];
        glBindTexture(GL_TEXTURE_2D, textId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
        _textures[itr]->setId(textId);
    }
}

void PicturePainter::setDataBuffer(uint8_t textId, const ImageQueue *buffer) {
    std::lock_guard<std::mutex> locker(_mtx);
    if (textId < (int)_textures.size() && textId >= 0) {
        _textures[textId]->initBuffer(buffer);
    }
}