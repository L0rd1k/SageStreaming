#pragma once

#include <atomic>

#include "callbacks/signal.h"
#include "definitions/local_definitions.h"
#include "image/image.h"
#include "image/image_queue.h"
#include "utils/log.h"

namespace sage {

class CamerasHandler {
public:
    CamerasHandler();
    virtual ~CamerasHandler();
    virtual bool start() = 0;
    virtual bool stop() = 0;
    virtual std::string getName() = 0;
    bool isStreaming();
    const ImageQueue* getQueue();
    virtual sage::ImageFormat getImageFormat();
    ccflow::Signal<const sage::swImage&> sig_imageRecieved;
protected:
    void triggerImage(sage::swImage& image);
    std::atomic<bool> _isStreaming;
    ImageQueue _queue;
};

}  // namespace sage