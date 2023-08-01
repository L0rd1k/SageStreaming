#pragma once

#include "cameras_handler.h"
#include "definitions/local_definitions.h"
#include "config/ini_parser.h"
//> Cameras
#include "ffmpeg/camera_ffmpeg.h"
#include "opencv/camera_opencv.h"
#include "gstreamer/camera_gstreamer.h"
//> Decoder
#include "decoder/ffmpeg/ffmpeg_decoder.h"
//> Decoder
#include "encoder/opencv/opencv_encoder.h"


namespace sage {

class CamerasCreator {
public:
    CamerasCreator() = default;
    virtual ~CamerasCreator() = default;
    static CamerasCreator& inst();

    std::shared_ptr<CamerasHandler> createCamera(sage::CamTypes camType = sage::CamTypes::Undefined,
                                 std::string name = std::string(), 
                                 sage::FFmpegType ffmpegType = sage::FFmpegType::Undefined, 
                                 sage::OpencvType opencvType = sage::OpencvType::Undefined);
    Decoder* createDecoder(sage::DecTypes dectype);
    Encoder* createEncoder(sage::EncTypes encType);

};

}  // namespace sage