#pragma once 

#include "structures/queue/circle_queue.h"
#include "image.h"

class ImageQueue : public CircleQueue<sage::swImage, sage::queueSize> {
public:
    ImageQueue(size_t maxByteSize) : 
    CircleQueue<sage::swImage, sage::queueSize>() {
        for(auto &img : _data) {
            img.memAllocate(maxByteSize);
        }
    } 
};