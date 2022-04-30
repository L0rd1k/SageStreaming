#pragma once

#include <atomic>
#include <mutex>
#include <iostream>

template <typename T, int SIZE>
class Queue {
public:
    Queue() : 
    _peak(-1), 
    _front(0) {
    };

    T& peak() {
        return _data[_peak];
    }

    const T& peak() const {
        return _data[_peak];
    }

    T& front() {
        return _data[_front];
    }

    const T& front() const {
        return _data[_front];
    }

protected:
    T _data[SIZE]; 
    std::mutex _locker;
    std::atomic<uint> _peak;
    std::atomic<uint> _front;
};