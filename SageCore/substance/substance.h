#pragma once

#include "cameras/cameras_creator.h"
#include "config/ini_parser.h"
#include "decoder/decoder.h"
#include "definitions/local_definitions.h"
#include "utils/elapsed_timer.h"

namespace sage {

class Substance {
public:
    Substance(short id, bool newOne = false);
    virtual ~Substance();

    void initConfig();
    bool initSubstance();
    bool isEnabled();
    bool enableSubstance();
    bool disableSubstance();
    void mainSubstanceLoop();
    void connectCallbacks();

    const ImageQueue* getImageQueue();
    CamerasHandler* getCamera();
    Decoder* getDecoder();
    short getId();

    void setCamReaderType(const sage::CamTypes type);
    void setCamDecoderType(const sage::DecTypes type);
    void setCamUrl(const std::string url);
    void setFFmpegCaptureType(const sage::RtspTransportType type);
    void setOpenCVCaptureType(const sage::CVCapType type);


    void onImageReceived(const sage::swImage& img);
    void onImageDecode(const sage::swImage& img);
    void onSubstanceInfoSend();
    void startCameraStreaming();

    std::vector<sage::Scope<void*> > callbacks;
    Decoder* _decoder = nullptr;  //> Camera decoder base
    Signal<const SubstanceInfo&> sig_sendSubstInfo;  //> State signal(called every second)
    Signal<const CameraState&> sig_sendSubstState;

private:
    Signal<const sage::swImage&> sig_imageDecoded;     //> Decoding signal(called after reading)
    std::thread _mainThread;  //> Main substance thread
    CamerasHandler* _camera = nullptr;              //> Camera base handler
    sage::Scope<SubstanceInfo> _subInfo = nullptr;  //> Current Substance State
    sage::Scope<CameraState> _state = nullptr;      //> Currnet CameraState
    sage::CamTypes _camType;  //> Camera Reader Type
    sage::DecTypes _decType;  //> Camera Decoder Type
    sage::RtspTransportType _transportType;
    sage::CVCapType _cvcaptureType;
    std::string _url;
    ElapsedTimer timer;  //> fps counter timer
    uint fps;            //> current fps value
    bool _isInited;      //> is substace inited;
    bool _inProcess;     //> is substance enabled;
    bool _isLoopOn;      //> is main loop running
    short _id;           //> substance identification
};

}  // namespace sage