#pragma once

#include <unistd.h>

#include <atomic>
#include <iostream>
#include <mutex>

#include "utils/log.h"

template <typename T, uint32_t SIZE>
class Queue {
public:
    Queue() : _peak(-1), _front(0){};

    T& peak() { return _data[_peak]; }

    const T& peak() const { return _data[_peak]; }

    T& front() { return _data[_front]; }

    const T& front() const { return _data[_front]; }

    T& getData(uint32_t pos) { return _data[pos]; }

protected:
    T _data[SIZE];
    std::mutex _locker;
    std::atomic<int> _peak;
    std::atomic<int> _front;
};