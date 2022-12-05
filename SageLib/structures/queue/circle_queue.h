#pragma once 

#include "queue.h"
#include "utils/log.h"

template <typename T, uint32_t SIZE = 3>
class CircleQueue : public Queue<T, SIZE> {
public:
    CircleQueue() : Queue<T, SIZE>() {
    }

    void push(const T& data) {
        std::lock_guard<std::mutex> lg(Queue<T, SIZE>::_locker);
        size_t pos = nextPos();
        Queue<T, SIZE>::_data[pos] = data;
        Queue<T, SIZE>::_peak = pos;   
    }

    void moveNext() {
        std::lock_guard<std::mutex> lg(Queue<T, SIZE>::_locker);
        Queue<T, SIZE>::_peak = nextPos();
    }

    T& next() {
        return Queue<T, SIZE>::_data[nextPos()];
    }

    const T& next() const {
        return Queue<T, SIZE>::_data[nextPos()];
    }

    virtual ~CircleQueue() {
    }

    const int getPos() const {
        return currentPos;
    }

protected:
    size_t nextPos() {
        size_t newPos = Queue<T,SIZE>::_peak + 1;
        return (newPos >= SIZE) ? currentPos = 0 : currentPos = newPos;
    }

private:
    const int _size = SIZE;
    size_t currentPos = -1;
};