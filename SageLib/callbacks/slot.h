#pragma once 

#include <functional>
#include "utils/log.h"

template<typename RET, typename ...Args>
class Slot {
public:
    Slot(std::function<RET(Args...)> callback) :
    _callback(callback) {
    };

    template<typename Class1, typename Class2>
    Slot(Class1 *inst, 
        RET(Class2::*callback)(Args...)) :
        _callback([this, inst, callback](Args&&... args) {
            return (inst->*callback)(std::forward<Args>(args)...);
        }) {
    }

    virtual ~Slot() {};

    virtual void operator()(Args&&... args) {
        try {
            _callback(std::forward<Args>(args)...);
        } catch (std::exception &e){
            Log() << "Error: " << e.what() << "\n";
            throw e;
        }
    }

protected:
    std::function<void(Args...)> _callback;
};
