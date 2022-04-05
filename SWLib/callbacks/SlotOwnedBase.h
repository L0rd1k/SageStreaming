#pragma once

#include "SlotOwnerBase.h"
#include "SlotOwned.h"
#include "SlotList.h"

class SlotOwnerBase;

class SlotOwnedBase {
public:
    SlotOwnedBase(SlotOwnerBase *owner) :
    _owner(owner) {};
    virtual ~SlotOwnedBase() = 0;
    SlotOwnerBase *getOwner() { return _owner; }
protected:
    SlotOwnerBase *_owner;
};

inline SlotOwnedBase::~SlotOwnedBase() {};