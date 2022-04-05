#pragma once

#include <mutex>
#include <vector>

#include "SlotBase.h"
#include "SlotOwned.h"
#include "SlotOwnedBase.h"
#include "SlotOwnerBase.h"

template <typename RET, typename ...Args>
class SlotList : public SlotOwnerBase {
public:
    SlotList(){};
    virtual ~SlotList() {
        std::lock_guard<std::mutex> lgActive(lockerActive);
        updateActive();
        for(auto &action : _active) {
            action->_owner = nullptr;
        }
    };

    SlotOwned<RET, Args...> *add(std::function<RET(Args...)> callback) {
        return new SlotOwned<RET, Args...>(this, callback);
    };

    template <typename CLS1, typename CLS2>
    SlotOwned<RET, Args...> *add(CLS1 *instance,
        ClassMethod<CLS2, RET, Args...> method) {
        return new SlotOwned<RET, Args...>(this, instance, method);
    };


protected:
    std::mutex _locker;
    
    const std::vector<SlotOwned<RET, Args...>*>& getSlots() {
        updateActive();
        return _active;
    };

private:
    virtual void addSlot(SlotOwnedBase *action) override {
        auto act = static_cast<SlotOwned<RET, Args...>*>(action);
        if(act) {
            addAction(act);
        } else {
            throw std::invalid_argument("Unknown SLOT received");
        }
    };

    virtual void removeSlot(SlotOwnedBase *action) override {
        auto act = static_cast<SlotOwned<RET, Args...>*>(action);
        if(act) {
            removeAction(act);
        } else {
            throw std::invalid_argument("Unknown SLOT received");
        }
    };

    virtual void addAction(SlotOwned<RET, Args...> *action) {
        std::lock_guard<std::mutex> lgChange(_locker);
        std::lock_guard<std::mutex> lg(lockerAppend);
        _append.push_back(action);
    }

    virtual void removeAction(SlotOwned<RET, Args...> *action) {
        std::lock_guard<std::mutex> lgChange(_locker);
        std::lock_guard<std::mutex> lg(lockerReduce);
        _reduce.push_back(action);
    }


    virtual void updateActive() {
        std::lock_guard<std::mutex> lgActive(lockerActive);
        {
            std::lock_guard<std::mutex> lgReduce(lockerReduce);
            for(size_t i = 0; i < _append.size(); i++) {
                for(auto iter = _active.begin(); iter != _active.end(); iter++) {
                    if(_reduce[i] == *iter) { 
                        _active.erase(iter);
                        break;
                    }
                }
            }
            _reduce.clear();
        }

        {
            std::lock_guard<std::mutex> lgAppend(lockerAppend);
            for(size_t i = 0; i < _append.size(); i++) {
                _active.push_back(_append[i]);
            }
            _append.clear();
        }
    }

    std::mutex lockerAppend;
    std::vector<SlotOwned<RET, Args...> *> _append;

    std::mutex lockerReduce;
    std::vector<SlotOwned<RET, Args...> *> _reduce;

    std::mutex lockerActive;
    std::vector<SlotOwned<RET, Args...> *> _active;
};