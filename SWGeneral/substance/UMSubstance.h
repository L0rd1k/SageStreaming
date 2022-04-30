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
    void mainSubstanceLoop();
    const ImageQueue* getImageQueue();
    CamerasHandler* getCamera();
private:
    std::thread _mainThread;
    short _id;
    CamerasHandler* _camera;
    bool _inProcess;  //> is Subsatance enabled;
    bool _isLoopOn;   //> is main looprunning
};