#pragma once

#include "CamerasHandler.h"
#include "definitions/LocalDefinitions.h"
#include "ffmpeg/CameraFFmpeg.h"

class CamerasCreator {
public:
    CamerasCreator() = default;
    virtual ~CamerasCreator() = default;
    static CamerasCreator& inst();
    CamerasHandler* createCamera(short id, cam::CamTypes camType);
};