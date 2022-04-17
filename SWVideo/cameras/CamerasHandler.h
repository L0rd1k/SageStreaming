#pragma once

#include <atomic>

#include "definitions/LocalDefinitions.h"

#include "utils/Log.h"

#include "image/Image.h"
#include "image/ImageQueue.h"

class CamerasHandler {
public:
    CamerasHandler();
    virtual ~CamerasHandler();
    virtual bool start() = 0;
    virtual bool stop() = 0;
    bool isStreaming();
protected:
    void triggerImage(img::swImage &image); 
    std::atomic<bool> _isStreaming;
    ImageQueue _queue;
};