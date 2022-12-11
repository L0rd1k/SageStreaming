#pragma once

#include "image/image.h"
#include "image/image_queue.h"

namespace sage {

class Encoder {
public:
    Encoder();
    virtual ~Encoder();
    virtual bool encode(const sage::swImage& in, sage::swImage& out) = 0;
    ImageQueue* getQueue();
protected:
    ImageQueue _queue;
};

}  // namespace sage