#pragma once

#include "cameras_handler.h"
#include "definitions/local_definitions.h"
//> Cameras
#include "ffmpeg/camera_ffmpeg.h"
#include "opencv/camera_opencv.h"
#include "gstreamer/camera_gstreamer.h"
//> Decoder
#include "config/ini_parser.h"
#include "decoder/ffmpeg/ffmpeg_decoder.h"



namespace sage {

class CamerasCreator {
public:
    CamerasCreator() = default;
    virtual ~CamerasCreator() = default;
    static CamerasCreator& inst();

    CamerasHandler* createCamera(sage::CamTypes camType = sage::CamTypes::Undefined,
                                 std::string name = std::string(), 
                                 sage::RtspTransportType ffmpegType = sage::RtspTransportType::Undefined, 
                                 sage::CVCapType opencvType = sage::CVCapType::Undefined);
    Decoder* createDecoder(sage::DecTypes dectype);
};

}  // namespace sage