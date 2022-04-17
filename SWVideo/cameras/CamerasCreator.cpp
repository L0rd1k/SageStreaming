#include "CamerasCreator.h"


CamerasCreator& CamerasCreator::inst() {
    static CamerasCreator instance;
    return instance;
}

CamerasHandler* CamerasCreator::createCamera(short id, cam::CamTypes camType) {
    CamerasHandler *camHandler = nullptr;
    switch(camType) {
        case cam::CamTypes::FFMPEG: {
            // camHandler = new CameraFFmpeg("/dev/video0", RtspTransportType::V4l);
            // camHandler = new CameraFFmpeg("/home/ilya/Видео/VasyaHome/ch0_2019.05.23_10.06.33.avi", RtspTransportType::Vid);
            camHandler = new CameraFFmpeg("/home/ilya/Видео/SlovakiaRobot/Sample.avi", RtspTransportType::Vid);
        }
        case cam::CamTypes::OPENCV: {

        }
        default:
            break;
    }
    return camHandler;
}