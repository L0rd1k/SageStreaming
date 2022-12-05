#pragma once

#include "queue.h"
#include "utils/log.h"

template <typename T, uint32_t SIZE = 3>
class StaticQueue : public Queue<T, SIZE> {
public:
    StaticQueue() : Queue<T, SIZE>() {
        Queue<T, SIZE>::_peak = 0;
    }

    T& next() {
        Queue<T, SIZE>::_peak = (Queue<T, SIZE>::_peak % capacity()); 
        return Queue<T, SIZE>::_data[Queue<T, SIZE>::_peak];
    }
    
    void moveNext() {
        std::lock_guard<std::mutex> lg(Queue<T, SIZE>::_locker);
        if(!full()) {
            Queue<T, SIZE>::_peak = (Queue<T, SIZE>::_peak % capacity()) + 1; 
        } else {
            Log::critical("Can't insert element to static queue");
        }
    }

    const uint32_t capacity() { return SIZE; }

    const bool empty() { return !size(); }

    uint32_t full() { return size() == capacity(); }

    const uint32_t size() {
        return abs(Queue<T, SIZE>::_peak - Queue<T, SIZE>::_front);
    }
};