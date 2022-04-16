#pragma once

#include "CamerasHandler.h"
#include "definitions/LocalDefinitions.h"

#include "ffmpeg/CameraFFmpeg.h"

class CamerasCreator {
public:
    CamerasCreator();
    virtual ~CamerasCreator();
    CamerasHandler* createCamera(cam::CamTypes camType);
};