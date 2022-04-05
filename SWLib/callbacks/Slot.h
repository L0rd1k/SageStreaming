#pragma once

#include <iostream>

template<class C, class R, class ...Args>
using ClassMethod = R (C::*)(Args...);

template<typename RET, typename ...Args>
class Slot : public SlotBase {
public:    
    Slot(std::function<RET(Args...)> func) :
        _func(func) {
    };

    template<typename CLS1, typename CLS2>
    Slot(CLS1 *instance,
            ClassMethod<CLS2, RET, Args...> method) :
        _func([this, instance, method](Args&&... args){
            return (instance->*method)(std::forward<Args>(args)...);
        }) {
    };

    virtual ~Slot() override {};
    
    virtual void operator()(Args&&... args){
        try {
            _func(std::forward<Args>(args)...);
        } catch (std::exception &e){
            std::cerr << "Error: " << e.what() << std::endl;
            throw e;
        }
    };


protected:
    std::function<void(Args...)> _func;
};

