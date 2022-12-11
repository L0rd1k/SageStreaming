#pragma once

#include "cameras/cameras_creator.h"
#include "decoder/decoder.h"
#include "encoder/encoder.h"
#include "definitions/local_definitions.h"
#include "utils/elapsed_timer.h"
#include "substance_config.h"

#include <thread>
#include <future>
#include <mutex>

namespace sage {

class Substance {
public:
    Substance(short id, bool newOne = false);
    virtual ~Substance();

    bool initSubstance();
    bool isEnabled();
    bool enableSubstance();
    bool disableSubstance();
    void mainSubstanceLoop();
    void startCameraStreaming();
    void connectCallbacks();

    const ImageQueue* getImageQueue();
    CamerasHandler* getCamera();
    Decoder* getDecoder();
    Encoder* getEncoder();
    sage::Scope<sage::SubstanceConfig>& getConfig();

    void onImageReceived(const sage::swImage& img);
    void onImageDecode(const sage::swImage& img);
    void onImageEncode(const sage::swImage& img);
    void onSubstanceInfoSend();

    std::vector<sage::Scope<void*> > callbacks;
    Decoder* _decoder = nullptr;  //> Camera decoder base
    Encoder* _encoder = nullptr;  //> Camera encoder base

    Signal<const SubstanceState&> sig_sendSubstInfo;  //> State signal(called every second)
    Signal<const CameraState&> sig_sendSubstState;

private:
    Signal<const sage::swImage&> sig_imageDecoded;    //> Decoding signal(called after reading)
    Signal<const sage::swImage&> sig_imageEncoded;     //> Encoding signal(called after decoding)


    sage::Scope<sage::SubstanceConfig> _subConfig;

    std::thread _mainThread;  //> Main substance thread
    CamerasHandler* _camera = nullptr;              //> Camera base handler
    sage::Scope<SubstanceState> _subState = nullptr;  //> Current Substance State
    sage::Scope<CameraState> _camState = nullptr;      //> Currnet CameraState

    ElapsedTimer timer;  //> fps counter timer
    ElapsedTimer tst;  //> fps counter timer


    uint fps;            //> current fps value

    bool _isInited;      //> is substace inited;
    bool _inProcess;     //> is substance enabled;
};

}  // namespace sage