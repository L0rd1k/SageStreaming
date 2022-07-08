#pragma once

#include "cameras_handler.h"
#include "definitions/local_definitions.h"
#include "ffmpeg/CameraFFmpeg.h"

namespace sage {

class CamerasCreator {
public:
    CamerasCreator() = default;
    virtual ~CamerasCreator() = default;
    static CamerasCreator& inst();
    CamerasHandler* createCamera(short id, cam::CamTypes camType);
};

}  // namespace sage