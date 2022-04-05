#pragma once 

#include "structures/queue/CircleQueue.h"
#include "Image.h"

class ImageQueue : public CircleQueue<img::swImage, img::queueSize> {
public:
    ImageQueue(size_t maxByteSize) : 
    CircleQueue<img::swImage, img::queueSize>() {
        for(auto &img : _data) {
            img.memAllocate(maxByteSize);
        }
    } 
};