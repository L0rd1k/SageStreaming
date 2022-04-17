#pragma once

#include "cameras/CamerasCreator.h"

class UMSubstance {
public:
    UMSubstance(short id);
    virtual ~UMSubstance();
    bool initSubstance();
    bool isEnabled();
    bool enableSubstance();
    bool disableSubstance();
private:
    short _id;
    CamerasHandler* _camera;
    bool _inProcess;
};