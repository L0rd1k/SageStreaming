#include "cameras_creator.h"

sage::CamerasCreator& sage::CamerasCreator::inst() {
    static CamerasCreator instance;
    return instance;
}

sage::CamerasHandler* sage::CamerasCreator::createCamera(short id, cam::CamTypes camType) {
    CamerasHandler* camHandler = nullptr;
    switch (camType) {
        case cam::CamTypes::FFMPEG: {
            // camHandler = new CameraFFmpeg("/home/ilya/Видео/ik.avi", RtspTransportType::Vid);
            camHandler = new CameraFFmpeg("/home/ilya/Видео/SlovakiaRobot/Sample.avi", RtspTransportType::Vid);
            break;
            // camHandler = new CameraFFmpeg("/dev/video1", RtspTransportType::V4l);
        }
        case cam::CamTypes::OPENCV: {
        }
        default:
            break;
    }
    return camHandler;
}

sage::Decoder* sage::CamerasCreator::createDecoder(sage::DecTypes decType) {
    Decoder* decoder = nullptr;
    switch (decType) {
        case sage::DecTypes::FFMPEG: {
            Log::info("Create FFMpeg Decoder");
            decoder = new FFmpegDecoder();
            break;
        }
        default:
            break;
    }
    return decoder;
}
