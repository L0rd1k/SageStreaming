#pragma once

#include "SlotOwnedBase.h"

class SlotOwnedBase;

class SlotOwnerBase {
template<class RET, class ... Args>
friend class SlotOwned;
public:
    virtual ~SlotOwnerBase() = 0;
protected:
    virtual void addSlot(SlotOwnedBase *slot) = 0;
    virtual void removeSlot(SlotOwnedBase *slot) = 0;
};

inline SlotOwnerBase::~SlotOwnerBase() {};