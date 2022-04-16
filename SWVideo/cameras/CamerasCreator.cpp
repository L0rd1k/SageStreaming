#include "CamerasCreator.h"

CamerasCreator::CamerasCreator() {
}

CamerasCreator::~CamerasCreator() {
}

CamerasHandler* CamerasCreator::createCamera(cam::CamTypes camType) {
    CamerasHandler *camHandler = nullptr;
    switch(camType) {
        case cam::CamTypes::FFMPEG: {
            // camHandler = new CameraFFmpeg("/home/ilya/Видео/ik.avi");
        }
        case cam::CamTypes::OPENCV: {

        }
        default:
            break;
    }
    return camHandler;
}