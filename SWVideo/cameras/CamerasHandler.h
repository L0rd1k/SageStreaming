#pragma once

#include <atomic>

#include "definitions/LocalDefinitions.h"
#include "image/Image.h"

class CamerasHandler {
public:
    CamerasHandler();
    virtual ~CamerasHandler();
    virtual bool start() = 0;
    virtual bool stop() = 0;
protected:
    void triggerImage(img::swImage &image); 
    std::atomic<bool> _isStreaming;
};