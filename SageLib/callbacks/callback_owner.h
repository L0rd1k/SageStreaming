#pragma once

#include "callback_list.h"
#include "slot.h"

template<typename RET, typename ...Args>
class CallbackList;

template<typename RET, typename ...Args>
class CallbackOwner : public Slot<RET, Args...> {
public:
    CallbackOwner(CallbackList<RET, Args...>* list, 
        std::function<RET(Args...)> callback) :
        Slot<RET, Args...>(callback), 
        _list(list) {
            _list->addSlot(this);
    };

    template<typename C1, typename C2>
    CallbackOwner(CallbackList<RET, Args...>* list,
        C1 *inst, RET(C2::*callback)(Args...)) :
        Slot<RET, Args...>(inst, callback),
        _list(list) {
            _list->addSlot(this);
    }

    virtual ~CallbackOwner() {
        if(_list){
            _list->removeSlot(this);
        }
    };

    CallbackOwner *getOwner() {
        return *this;
    }
    
private:
    CallbackList<RET, Args...>* _list;

};