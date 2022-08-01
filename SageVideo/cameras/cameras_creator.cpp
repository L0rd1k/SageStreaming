#include "cameras_creator.h"

sage::CamerasCreator& sage::CamerasCreator::inst() {
    static CamerasCreator instance;
    return instance;
}

sage::CamerasHandler* sage::CamerasCreator::createCamera(short id, cam::CamTypes camType) {
    CamerasHandler* camHandler = nullptr;
    switch (camType) {
        case cam::CamTypes::FFMPEG: {
            /** @todo Remove hardcode. Add xml parser. **/
            if(id == 3) {
                camHandler = new CameraFFmpeg("/home/ilya/Видео/SlovakiaRobot/Sample.avi", RtspTransportType::Vid);
            } else if (id == 0) {
                camHandler = new CameraFFmpeg("/home/ilya/Видео/Полеты/Voskresensk/TV/ch0_t.avi", RtspTransportType::Vid);
            } else if (id == 1) {
                camHandler = new CameraFFmpeg("/home/ilya/Видео/Полеты/Протасово/4.avi", RtspTransportType::Vid);
            } else if (id == 2) {
                camHandler = new CameraFFmpeg("/home/ilya/Видео/Полеты/Протасово/3.avi", RtspTransportType::Vid);
            }
            break;
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
