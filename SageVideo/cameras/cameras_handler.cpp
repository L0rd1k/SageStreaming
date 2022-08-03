#include "cameras_handler.h"

sage::CamerasHandler::CamerasHandler() :
_isStreaming(false),
_queue(1000) {
}

sage::CamerasHandler::~CamerasHandler() {
}

void sage::CamerasHandler::triggerImage(sage::swImage &image) {
    _queue.moveNext();
    sig_imageRecieved.emit(image);
}

bool sage::CamerasHandler::isStreaming() {
    return _isStreaming;
}

const ImageQueue* sage::CamerasHandler::getQueue() {
    return &_queue;
} 

sage::ImageFormat sage::CamerasHandler::getImageFormat() {
    return sage::ImageFormat::Undefined;
}
