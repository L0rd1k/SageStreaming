#include "cameras_creator.h"

sage::CamerasCreator& sage::CamerasCreator::inst() {
    static CamerasCreator instance;
    return instance;
}

sage::CamerasHandler* sage::CamerasCreator::createCamera(sage::CamTypes camType,
                                                         std::string url,
                                                         sage::RtspTransportType ffmpegType,
                                                         sage::CVCapType opencvType) {
    CamerasHandler* camHandler = nullptr;
    switch (camType) {
        case sage::CamTypes::FFMPEG: {
            camHandler = new CameraFFmpeg(url, ffmpegType);
            break;
        }
        case sage::CamTypes::OPENCV: {
            camHandler = new CameraOpencv(url, opencvType);
            break;
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
