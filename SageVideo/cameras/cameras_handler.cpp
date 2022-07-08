#include "cameras_handler.h"

sage::CamerasHandler::CamerasHandler() :
_isStreaming(false),
_queue(1000) {
}

sage::CamerasHandler::~CamerasHandler() {
}

void sage::CamerasHandler::triggerImage(img::swImage &image) {
    _queue.moveNext();
    sig_imageRecieved.emit(image);

}

bool sage::CamerasHandler::isStreaming() {
    return _isStreaming;
}

const ImageQueue* sage::CamerasHandler::getQueue() {
    return &_queue;
} 

img::ImageFormat sage::CamerasHandler::getImageFormat() {
    return img::ImageFormat::Undefined;
}
