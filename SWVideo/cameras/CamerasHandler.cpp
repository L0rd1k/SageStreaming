#include "CamerasHandler.h"

CamerasHandler::CamerasHandler() :
_isStreaming(false),
_queue(1000) {
}

CamerasHandler::~CamerasHandler() {
}

void CamerasHandler::triggerImage(img::swImage &image) {
    
    // Log() << "-----------";
    // Log() << "Resolution: " << image.header().imgSize.width() << "x" << image.header().imgSize.height(); 
    // Log() << "Format: " << toString(image.header().imgFormat);
    // Log() << "Source: " << toString(image.header().imgSourceType);
    _queue.moveNext();
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
