#include "CamerasCreator.h"

CamerasCreator::CamerasCreator() {
}

CamerasCreator::~CamerasCreator() {
}

CamerasHandler* CamerasCreator::createCamera(cam::CamTypes camType) {
    CamerasHandler *camHandler = nullptr;
    switch(camType) {
        case cam::CamTypes::FFMPEG: {
            camHandler = new CameraFFmpeg("/home/ilya/Видео/VasyaHome/ch0_2019.05.22_21.28.14.avi", RtspTransportType::Udp);
        }
        case cam::CamTypes::OPENCV: {

        }
        default:
            break;
    }
    return camHandler;
}