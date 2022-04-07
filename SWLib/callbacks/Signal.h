#pragma once

#include "CallbackList.h"

template <class ...Args>
class Signal : public CallbackList<void, Args...> {
public:
    Signal() : CallbackList<void, Args...>() {};

    virtual ~Signal()  {};

    void emit(Args&&... args) {
        std::lock_guard<std::mutex>(Signal::locker);
        const auto& list = Signal::getActiveSlotsList();
        for(const auto& action : list) {
            (*action)(std::forward<Args>(args)...);
        }
    }

};