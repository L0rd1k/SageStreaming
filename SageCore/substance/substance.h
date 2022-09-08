#pragma once

#include "cameras/cameras_creator.h"
#include "config/ini_parser.h"
#include "decoder/decoder.h"
#include "definitions/local_definitions.h"
#include "utils/elapsed_timer.h"

namespace sage {

class Substance {
public:
    Substance(short id, sage::CamTypes camType, sage::DecTypes decType,
              std::string url = std::string(),
              sage::RtspTransportType transportType = sage::RtspTransportType::Undefined,
              sage::CVCapType cvcaptureType = sage::CVCapType::Undefined,
              bool newOne = false);
    virtual ~Substance();

    bool initSubstance(std::string url, sage::CamTypes& camType, sage::DecTypes& decType,
                       sage::RtspTransportType transportType, sage::CVCapType cvcaptureType, bool newOne = false);
    bool isEnabled();
    bool enableSubstance();
    bool disableSubstance();
    void mainSubstanceLoop();
    void connectCallbacks();

    const ImageQueue* getImageQueue();
    CamerasHandler* getCamera();
    Decoder* getDecoder();
    short getId();

    void onImageReceived(const sage::swImage& img);
    void onImageDecode(const sage::swImage& img);
    void onSubstanceInfoSend();
    void startCameraStreaming();

    std::vector<sage::Scope<void*> > callbacks;
    Decoder* _decoder = nullptr;                     //> Camera decoder base
    Signal<const SubstanceInfo&> sig_sendSubstInfo;  //> State signal(called every second)
    Signal<const CameraState&> sig_sendSubstState;

private:
    Signal<const sage::swImage&> sig_imageDecoded;  //> Decoding signal(called after reading)
    std::thread _mainThread;                        //> Main substance thread
    CamerasHandler* _camera = nullptr;              //> Camera base handler
    sage::Scope<SubstanceInfo> _subInfo = nullptr;  //> Current Substance State
    sage::Scope<CameraState> _state = nullptr;      //> Currnet CameraState

    sage::CamTypes _camType;  //> Camera Reader Type
    sage::DecTypes _decType;  //> Camera Decoder Type
    ElapsedTimer timer;       //> fps counter timer
    uint fps;                 //> current fps value
    bool _isInited;           //> is substace inited;
    bool _inProcess;          //> is substance enabled;
    bool _isLoopOn;           //> is main loop running
    short _id;                //> substance identification
};

}  // namespace sage