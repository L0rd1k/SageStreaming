#pragma once

#include <vector>
#include <thread>
#include <mutex>

#include "CallbackOwner.h"

template <typename RET, typename ...Args>
class CallbackList {
public:
    CallbackList() {};

    virtual ~CallbackList() {
        std::lock_guard<std::mutex> lg(lockerActive);
        updateActiveSlots();
        for(auto &action : _active) {
            action = nullptr;
            delete action;
        }
    };

    CallbackOwner<RET, Args...>* connect(std::function<RET(Args...)> callback) {
        return new CallbackOwner<RET, Args... >(this, callback);
    };

    template<typename Class1, typename Class2>
    CallbackOwner<RET, Args...>* connect(Class1 *inst,
        RET(Class2::*callback)(Args...)) {
        return new CallbackOwner<RET, Args...>(this, inst, callback);
    };

    const std::vector<CallbackOwner<RET, Args...>*>& getActiveSlotsList(){
        updateActiveSlots();
        return _active;
    };

    void addSlot(CallbackOwner<RET, Args...> *callback) {
        if(callback) {
            std::lock_guard<std::mutex> lg(locker);
            std::lock_guard<std::mutex> lgApp(lockerAppend);
            _append.push_back(callback);
        }
    };

    void removeSlot(CallbackOwner<RET, Args...> *callback) {
        if(callback) {
            std::lock_guard<std::mutex> lg(locker);
            std::lock_guard<std::mutex> lgRed(lockerReduce);
            _reduce.push_back(callback);
        }
    };
protected:
    std::mutex locker;

private:

    void appendNewSlots() {
        std::lock_guard<std::mutex> lg(lockerAppend);
        for(size_t i = 0; i < _append.size(); i++) {
            _active.push_back(_append[i]);
        }
        _append.clear();
    };

    void reduceActiveSlots() {
        std::lock_guard<std::mutex> lg(lockerReduce);
        for(size_t i = 0; i < _append.size(); i++) {
            for(auto itr = _active.begin(); itr != _active.end(); itr++) {
                if(_reduce[i] == *itr) {
                    _active.erase(itr);
                    break;
                }
            }
        }
        _reduce.clear();
    };

    void updateActiveSlots() {
        Log() << "Update active slots";
        std::lock_guard<std::mutex> lg(lockerActive);
        {
            reduceActiveSlots();
        }
        {
            appendNewSlots();
        }
    };

private:
    std::mutex lockerAppend;
    std::vector<CallbackOwner<RET, Args...> *> _append;
    
    std::mutex lockerReduce;
    std::vector<CallbackOwner<RET, Args...> *> _reduce;

    std::mutex lockerActive;
    std::vector<CallbackOwner<RET, Args...> *> _active;

};