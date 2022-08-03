#pragma once

#include "callback_list.h"

template <class ...Args>
class Signal : public CallbackList<void, Args...> {
public:
    Signal() : CallbackList<void, Args...>() {};

    virtual ~Signal()  {};

    void emit(Args&&... args) {
        std::lock_guard<std::mutex>(Signal::locker);
        auto& list = Signal::getActiveSlotsList();
        for (uint i = 0; i < list.size(); ++i){
            auto action = list[i];
            (*action)(std::forward<Args>(args)...);
        }
    }

};