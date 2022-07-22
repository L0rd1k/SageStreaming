#pragma once

#include "definitions/sage_definitions.h"

#include "cameras/cameras_creator.h"
#include "decoder/decoder.h"

namespace sage {

class Substance {
public:
    Substance(short id);
    virtual ~Substance();

    bool initSubstance();
    bool isEnabled();
    bool enableSubstance();
    bool disableSubstance();
    void mainSubstanceLoop();
    void connectCallbacks();


    const ImageQueue* getImageQueue();
    CamerasHandler* getCamera();
    Decoder* getDecoder();


    void onImageReceived(const img::swImage& img);
    void startCameraStreaming();

    std::vector<sage::Scope<void*> > callbacks;
    Decoder* _decoder = nullptr;

private:
    Signal<const img::swImage&> sig_imageDecoded;

    std::thread _mainThread;
    
    CamerasHandler* _camera = nullptr;

    bool _inProcess;  //> is substance enabled;
    bool _isLoopOn;   //> is main loop running
    short _id;
};

}  // namespace sage