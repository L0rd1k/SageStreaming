#pragma once

#include "SlotList.h"
#include "SlotOwned.h"


template<typename ...Args>
class Signal : public SlotList<void, Args...> {
public:
    Signal() : SlotList<void, Args...>(){};
    virtual ~Signal() {};
    void emit(Args&&... args) {
        std::lock_guard<std::mutex>(Signal::_locker);
        const std::vector<SlotOwned<void, Args...>*> &list = Signal::getSlots();
        
        for(size_t i = 0; i < list.size(); i++) {
            SlotOwned<void, Args...>* action = list[i];
            (*action)(std::forward<Args>(args)...);
        }
    }
};