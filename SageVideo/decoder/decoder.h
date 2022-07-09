#pragma once

#include "image/Image.h"
#include "image/ImageQueue.h"

namespace sage {

class Decoder {
public:
    Decoder();
    virtual ~Decoder();
    virtual bool decode(const img::swImage& in, img::swImage& out) = 0;
    ImageQueue* getQueue();
protected:
    ImageQueue _queue;
};

}  // namespace sage