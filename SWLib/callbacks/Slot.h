#pragma once 

#include <functional>

template<typename RET, typename ...Args>
class Slot {
public:
    Slot(std::function<RET(Args...)> callback) {
        _callback = callback;
    };

    template<typename Class1, typename Class2>
    Slot(Class1 *inst, 
        RET(Class2::*callback)(Args...)) :
        _callback([this, inst, callback](Args&&... args) {
            return (inst->*callback)(std::forward<Args>(args)...);
        }) {
    };

    virtual ~Slot() {};

    void operator()(Args&&... args) {
        _callback(std::forward<Args>(args)...);
    };

protected:
    std::function<RET(Args...)> _callback;
};
