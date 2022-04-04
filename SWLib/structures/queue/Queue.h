#pragma once

#include <atomic>
#include <mutex>

template <typename T, int SIZE>
class Queue {
    Queue() {
        _data = new T[SIZE];
        _capacity = SIZE;
        _count = 0;
        _front = 0;
        _back = -1
    }

    virtual ~Queue() {
    } 

    T* front(T item) {
    }

    T* back();

    bool empty() {
        return size() == 0;
    }

    size_t size() {
        return _count;
    }

    bool full() {
        return size() == _capacity;
    }

    bool push(T item) {
        if(!full()) {
           _back = (_back + 1) % _capacity;
           _data[_back] = item;
           _count++;
            return true;
        }
        return false;
    }

    bool pop() {
        if(!empty()) {
            
        }
    }
    bool swap();
protected:
    T *_data;
private:
    size_t _capacity;
    size_t _count;
    size_t _front;
    size_t _back;
};