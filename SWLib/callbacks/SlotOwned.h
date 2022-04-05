#pragma once

#include "SlotOwnedBase.h"
#include "SlotList.h"
#include "Slot.h"

template<class RET, class ... Args>
class SlotList;

template <typename RET, typename ...Args>
class SlotOwned : public Slot<RET, Args...>, public SlotOwnedBase {
friend SlotList<RET, Args...>;
public:
    SlotOwned(SlotOwnerBase *owner, 
        std::function<RET(Args...)> func) :
        Slot<RET, Args...>(func),
        SlotOwnedBase(owner) {
            owner->addSlot(this);
    };

    template <typename CLS1, typename CLS2>
    SlotOwned(SlotOwnerBase *owner, 
        CLS1 *instance,
        ClassMethod<CLS2, RET, Args...> method) :
        Slot<RET, Args...>(instance, method),
        SlotOwnedBase(owner) {
            owner->addSlot(this);
    };

    virtual ~SlotOwned() override {
        if(_owner) {
            _owner->removeSlot(this);
        }
    };

};