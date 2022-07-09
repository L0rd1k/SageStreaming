#include "decoder.h"

sage::Decoder::Decoder()
    : _queue(1000) {
}

sage::Decoder::~Decoder() {
}

ImageQueue* sage::Decoder::getQueue() {
    return &_queue;
}
