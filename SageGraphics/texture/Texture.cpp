#include "Texture.h"

Texture::Texture(GLuint id) :
_id(id),
_queueData(nullptr) {
}

Texture::~Texture() {
}

void Texture::setId(GLuint id) {
    _id = id;
}

void Texture::initBuffer(const ImageQueue* data) {
    _queueData = data;
}

bool Texture::getLastDataFromQueue() {
    Log::info("Get last data from queue");
    if(_queueData) {
        auto &data = _queueData->peak();
        Log() << "Size: " << data->imgSize.height() << " x " << data->imgSize.width();
        Log() << "Source type: " << toString(data->imgSourceType);
        Log() << "Format: " << toString(data->imgFormat);

        if(data->imgSize.isValid()) {
            // cv::Mat greyImg = cv::Mat( data->imgSize.width(),  data->imgSize.height(), CV_8UC3, data.data());
            // cv::imwrite("/home/ilya/Test.png", greyImg);

            // glBindTexture(GL_TEXTURE_2D, _id);
            // glTexImage2D(GL_TEXTURE_2D, 0, 420, 
            //     data->imgSize.width(), data->imgSize.height(),
            //     0, GL_LUMINANCE, GL_UNSIGNED_BYTE, data.data());
            return true;
        }
    }
    return false;
}

void Texture::draw(int x, int y, um::Size<int> sz) {
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
