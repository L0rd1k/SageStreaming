#pragma once 

#include "Queue.h"

template <typename T, int SIZE = 3>
class CircleQueue : public Queue<T, SIZE> {
    CircleQueue() : Queue<T, SIZE>() {
    }
    virtual ~CircleQueue() {
    }
};