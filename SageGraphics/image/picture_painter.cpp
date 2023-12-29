#include "picture_painter.h"

PicturePainter::PicturePainter(uint8_t textCount)
    : _isInited(false),
      _textures(textCount, nullptr) {
    allocateTextures(textCount);
}

PicturePainter::~PicturePainter() {
}

void PicturePainter::allocateTextures(size_t textSize) {
    std::lock_guard<std::mutex> locker(_mtx);
    _textures.assign(textSize, nullptr);
    for (auto itr = 0; itr < textSize; itr++) {
        _textures[itr] = std::make_shared<Texture>(-1);
    }
}

void PicturePainter::createTexture() {
    std::lock_guard<std::mutex> locker(_mtx);
    glEnable(GL_TEXTURE_2D);
    Log::critical("Tsize:", _textures.size(), "_Tsize:", textures.size());
    textures.push_back(_textures.size() + 1);
    _textures.push_back(std::make_shared<Texture>(textures.back()));
    glBindTexture(GL_TEXTURE_2D, textures.back());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glDisable(GL_TEXTURE_2D);
    Log::critical("New Tsize:", _textures.size(), "New _Tsize:", textures.size());
}

void PicturePainter::clearTextures() {
    glEnable(GL_TEXTURE_2D);
    glDeleteTextures(_textures.size(), textures.data());
    if (!_textures.empty()) {
        _textures.clear();
    }
    if (!textures.empty()) {
        textures.clear();
    }
    glDisable(GL_TEXTURE_2D);
}

void PicturePainter::popTexture(uint textureId) {
    glEnable(GL_TEXTURE_2D);
    glDeleteTextures(1, &textures[textureId + 1]);
    if (!_textures.empty()) {
        _textures.pop_back();
    }
    if (!textures.empty()) {
        textures.pop_back();
    }
    glDisable(GL_TEXTURE_2D);
}

uint8_t PicturePainter::getTexturesCount() {
    std::lock_guard<std::mutex> locker(_mtx);
    return _textures.size();
}

void PicturePainter::show(const sage::Size<int>& winSize, sage::Size<int>& textSize) {
    unsigned char pix[4] = {0, 0, 0, 255};
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);   
    // if (_textures[0]->getLastDataFromQueue()) {
    //     retrieveTextureSize(textSize);
    //     double ratioX = winSize.width() / (float)textSize.width();
    //     double ratioY = winSize.height() / (float)textSize.height();
    //     double ratio = (ratioX < ratioY) ? ratioX : ratioY;
    //     double viewWidth = textSize.width() * ratio;
    //     double viewHeight = textSize.height() * ratio;
    //     double viewX = (winSize.width() - textSize.width() * ratio) / 2;
    //     double viewY = (winSize.height() - textSize.height() * ratio) / 2;
    //     _textures[0]->draw(viewX, viewY, sage::Size<int>(viewWidth, viewHeight));
    //     // _textures[0]->draw(0, 0, sage::Size<int>(winSize.width(), winSize.height()));
    // }



    int x[4] = {0, winSize.width() / 2, 0, winSize.width() / 2};
    int y[4] = {winSize.height() / 2, winSize.height() / 2, 0, 0};
    for (auto i = 0; i < _textures.size(); i++) {
        if (_textures[i]->getLastDataFromQueue()) {
            retrieveTextureSize(textSize);
            double ratioX = (winSize.width() / 2) / (float)textSize.width();
            double ratioY = (winSize.height() / 2) / (float)textSize.height();
            double ratio = (ratioX < ratioY) ? ratioX : ratioY;
            double viewWidth = textSize.width() * ratio;
            double viewHeight = textSize.height() * ratio;
            double viewX = x[i] + (winSize.width() / 2 - textSize.width() * ratio) / 2;
            double viewY = y[i] + (winSize.height() / 2  - textSize.height() * ratio) / 2;
            _textures[i]->draw(viewX, viewY, sage::Size<int>(viewWidth, viewHeight));
        }
    }

    /** SPLITTER **/
    const int thickness = 2;
    glLineWidth((GLfloat)thickness);
    glColor3f(1, 0, 0);
    glBegin(GL_LINES);
    glVertex2i(winSize.width() / 2, 0);
    glVertex2i(winSize.width() / 2, winSize.height());
    glEnd();
    glBegin(GL_LINES);
    glVertex2i(0, winSize.height() / 2);
    glVertex2i(winSize.width(), winSize.height() / 2);
    glEnd();
    glRasterPos3f(0, 1, 0);
    glDrawPixels(1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pix);
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
    // glDisable(GL_TEXTURE_2D);
}

void PicturePainter::retrieveTextureSize(sage::Size<int> &size) {
    GLint textureWidth, textureHeight;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &textureWidth);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &textureHeight);
    size.setHeight(textureHeight);
    size.setWidth(textureWidth);
}

void PicturePainter::reinitTextures() {
    std::lock_guard<std::mutex> locker(_mtx);
    glEnable(GL_TEXTURE_2D);
    textures.assign(_textures.size(), 0);
    std::iota(std::begin(textures), std::end(textures), 1);
    for (uint itr = 0; itr < _textures.size(); itr++) {
        glBindTexture(GL_TEXTURE_2D, textures[itr]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
        _textures[itr]->setId(textures[itr]);
    }
    glDisable(GL_TEXTURE_2D);
}

std::vector<std::shared_ptr<Texture>> &PicturePainter::getTextures() {
    return _textures;
}

void PicturePainter::setDataBuffer(uint8_t textId, const ImageQueue *buffer) {
    std::lock_guard<std::mutex> locker(_mtx);
    if (textId < (int)_textures.size() && textId >= 0) {
        _textures[textId]->initBuffer(buffer);
    }
}