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
            camHandler = new CameraFFmpeg("/home/ilya/ik.avi", RtspTransportType::Vid);
            // camHandler = new CameraFFmpeg("/home/ilya/Видео/SlovakiaRobot/Sample.avi", RtspTransportType::Vid);
            // camHandler = new CameraFFmpeg("/dev/video1", RtspTransportType::V4l);

        }
        case cam::CamTypes::OPENCV: {

        }
        default:
            break;
    }
    return camHandler;
}