#include "cameras_creator.h"

sage::CamerasCreator& sage::CamerasCreator::inst() {
    static CamerasCreator instance;
    return instance;
}

sage::CamerasHandler* sage::CamerasCreator::createCamera(sage::CamTypes camType,
                                                         std::string url,
                                                         sage::FFmpegType ffmpegType,
                                                         sage::OpencvType opencvType) {
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
        case sage::CamTypes::GSTREAMER: {
            camHandler = new GstreamerCamera(url);
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

sage::Encoder* sage::CamerasCreator::createEncoder(sage::EncTypes encType) {
    Encoder* encoder = nullptr;
    switch (encType) {
        case sage::EncTypes::OPENCV: {
            Log::info("Create OPENCV jpeg Encoder");
            encoder = new OpencvEncoder();
            break;
        }
        default:
            break;
    }
    return encoder;    
}