#include "texture.h"

#include "opencv2/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include <unistd.h>
Texture::Texture(GLuint id) : _id(id),
                              _queueData(nullptr) {
}

Texture::~Texture() {
}

void Texture::setId(GLuint id) {
    _id = id;
}

void Texture::initBuffer(const ImageQueue* data) {
    Log::warning("Init buffer");
    _queueData = data;
}

bool Texture::getLastDataFromQueue() {
    if (_queueData != nullptr) {
        auto& data = _queueData->peak();
        if (data->imgSize.isValid()) {
            GLenum format = gl_GetColorType(data->imgColorType);
            GLenum internalFormat = gl_GetInternalFormat(data->imgColorType);
            if (format == GL_NONE || internalFormat == GL_NONE) {
                // Log() << "Unknown/Unsupported openGL pixel format" << toString(data->imgFormat);
                return false;
            }
            glBindTexture(GL_TEXTURE_2D, _id);
            if (_lastSize.height() == data->imgSize.height() && _lastSize.width() == data->imgSize.width() && _lastFormat == format) {
                glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, data->imgSize.width(), data->imgSize.height(),
                                format, GL_UNSIGNED_BYTE, data.data());
            } else {
                glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, data->imgSize.width(), data->imgSize.height(), 0,
                             format, GL_UNSIGNED_BYTE, data.data());
            }
            _lastSize = data->imgSize;
            _lastFormat = format;
            return true;
        }
    }
    return false;
}

void Texture::draw(int x, int y, sage::Size<int> sz) {
    glBindTexture(GL_TEXTURE_2D, _id);
    glBegin(GL_QUADS);

    glTexCoord2f(0, 0);
    glVertex2f(x, y);
    glTexCoord2f(1, 0);
    glVertex2f(x + sz.width(), y);
    glTexCoord2f(1, 1);
    glVertex2f(x + sz.width(), y + sz.height());
    glTexCoord2f(0, 1);
    glVertex2f(x, y + sz.height());

    glEnd();
}
