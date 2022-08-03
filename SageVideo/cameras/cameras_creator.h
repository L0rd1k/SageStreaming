#pragma once

#include "cameras_handler.h"
#include "definitions/local_definitions.h"
#include "definitions/sage_definitions.h"
//> Cameras
#include "ffmpeg/camera_ffmpeg.h"
#include "opencv/camera_opencv.h"
//> Decoder
#include "decoder/ffmpeg/ffmpeg_decoder.h"

namespace sage {

class CamerasCreator {
public:
    CamerasCreator() = default;
    virtual ~CamerasCreator() = default;
    static CamerasCreator& inst();

    CamerasHandler* createCamera(short id, sage::CamTypes camType);
    Decoder* createDecoder(sage::DecTypes dectype);


};

}  // namespace sage