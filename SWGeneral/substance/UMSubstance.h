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
    void connectCallbacks();
    const ImageQueue* getImageQueue();
    CamerasHandler* getCamera();

    void onImageReceived(const img::swImage& img);

    void startCameraStreaming();
    
    std::vector<std::unique_ptr<void*> > callbacks;
private:
    std::thread _mainThread;
    short _id;
    CamerasHandler* _camera;
    bool _inProcess;  //> is Subsatance enabled;
    bool _isLoopOn;   //> is main looprunning
};