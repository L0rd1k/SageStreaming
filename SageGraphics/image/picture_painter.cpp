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
    glEnable(GL_TEXTURE_2D);
    Log::critical("Tsize:", _textures.size() + 1);
    textures.push_back(_textures.size() + 1);
    _textures.push_back(std::make_shared<Texture>(textures.back()));
    glGenTextures(1, &textures.back());
    glBindTexture(GL_TEXTURE_2D, textures.back());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glDisable(GL_TEXTURE_2D);
}

uint8_t PicturePainter::getTexturesCount() {
    return _textures.size();
}

void PicturePainter::show(sage::Size<int> size) {
    unsigned char pix[4] = {0, 0, 0, 255};
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    // std::cout << size.width() << " " << size.height() << std::endl;
    // if (_textures[0]->getLastDataFromQueue()) {
    //     _textures[0]->draw(0, 0, sage::Size<int>(size.width(), size.height()));
    // }

    int x[4] = {0, size.width() / 2, 0, size.width() / 2};
    int y[4] = {size.height() / 2, size.height() / 2, 0, 0};
    for (auto i = 0; i < _textures.size(); i++) {
        if (_textures[i]->getLastDataFromQueue()) {
            // Log::trace("SHOW", i, _textures.size(), size.width(), size.height());
            _textures[i]->draw(x[i], y[i], sage::Size<int>(size.width()/2, size.height()/2));
            // _textures[i]->draw(0, 0, sage::Size<int>(size.width(), size.height()));
        }
    }

    /** SPLITTER **/
    // const int thickness = 2;
    // glLineWidth((GLfloat)thickness);
    // glColor3f(1, 0, 0);
    // glBegin(GL_LINES);
    // glVertex2i(size.width() / 2, 0);
    // glVertex2i(size.width() / 2, size.height());
    // glEnd();
    // glBegin(GL_LINES);
    // glVertex2i(0, size.height() / 2);
    // glVertex2i(size.width(), size.height() / 2);
    // glEnd();
    // glRasterPos3f(0, 1, 0);
    // glDrawPixels(1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pix);
}

void PicturePainter::initTextures() {
    std::lock_guard<std::mutex> locker(_mtx);
    glEnable(GL_TEXTURE_2D);
    textures.assign(_textures.size(), 0);
    std::iota(std::begin(textures), std::end(textures), 1);
    glGenTextures(_textures.size(), textures.data());
    for (uint itr = 0; itr < _textures.size(); itr++) {
        glBindTexture(GL_TEXTURE_2D, textures[itr]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
        _textures[itr]->setId(textures[itr]);
    }
}

void PicturePainter::setDataBuffer(uint8_t textId, const ImageQueue *buffer) {
    std::lock_guard<std::mutex> locker(_mtx);
    if (textId < (int)_textures.size() && textId >= 0) {
        _textures[textId]->initBuffer(buffer);
    }
}