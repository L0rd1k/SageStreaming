#include "CamerasHandler.h"

CamerasHandler::CamerasHandler() :
_isStreaming(false),
_queue(1000) {
}

CamerasHandler::~CamerasHandler() {
}

void CamerasHandler::triggerImage(img::swImage &image) {
    _queue.moveNext();
    sig_imageRecieved.emit(image);

}

bool CamerasHandler::isStreaming() {
    return _isStreaming;
}

const ImageQueue* CamerasHandler::getQueue() {
    return &_queue;
} 

img::ImageFormat CamerasHandler::getImageFormat() {
    return img::ImageFormat::Undefined;
}
