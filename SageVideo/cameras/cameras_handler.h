#pragma once

#include <atomic>

#include "callbacks/Signal.h"
#include "definitions/local_definitions.h"
#include "image/Image.h"
#include "image/ImageQueue.h"
#include "utils/Log.h"

namespace sage {

class CamerasHandler {
public:
    CamerasHandler();
    virtual ~CamerasHandler();
    virtual bool start() = 0;
    virtual bool stop() = 0;
    bool isStreaming();
    const ImageQueue* getQueue();
    virtual img::ImageFormat getImageFormat();

    Signal<const img::swImage&> sig_imageRecieved;

protected:
    void triggerImage(img::swImage& image);
    std::atomic<bool> _isStreaming;
    ImageQueue _queue;
};

}  // namespace sage