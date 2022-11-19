#include "encoder.h"

sage::Encoder::Encoder() 
    : _queue(1000) {
}

sage::Encoder::~Encoder() {
}

ImageQueue* sage::Encoder::getQueue() {
    return &_queue;
}
