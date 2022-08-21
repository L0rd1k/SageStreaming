#pragma once

#include "cameras/cameras_creator.h"
#include "decoder/decoder.h"
#include "definitions/sage_definitions.h"
#include "utils/elapsed_timer.h"

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

    void onImageReceived(const sage::swImage& img);
    void onImageDecode(const sage::swImage& img);

    void startCameraStreaming();

    std::vector<sage::Scope<void*> > callbacks;
    Decoder* _decoder = nullptr;

private:
    Signal<const sage::swImage&> sig_imageDecoded;
    std::thread _mainThread;
    CamerasHandler* _camera = nullptr;

    ElapsedTimer timer;  //> fps counter timer
    uint fps;            //> current fps value
    bool _isInited;      //> is substace inited;
    bool _inProcess;     //> is substance enabled;
    bool _isLoopOn;      //> is main loop running
    short _id;           //> substance identification
};

}  // namespace sage