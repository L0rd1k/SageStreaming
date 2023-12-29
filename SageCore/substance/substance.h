#pragma once

#include <future>
#include <mutex>
#include <thread>

#include "callbacks/callback.h"
#include "cameras/cameras_creator.h"
#include "decoder/decoder.h"
#include "definitions/local_definitions.h"
#include "encoder/encoder.h"
#include "substance_config.h"
#include "utils/elapsed_timer.h"

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
    void updateConfig(const sage::SubstanceState& camState);

    void extractConfigToState();

    const ImageQueue* getImageQueue();
    std::shared_ptr<CamerasHandler> getCamera();
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
    ccflow::Signal<const SubstanceState&> sigSendSubstParams;

private:
    ccflow::Signal<const sage::swImage&> sig_imageDecoded;  //> Decoding signal(called after reading)
    ccflow::Signal<const sage::swImage&> sig_imageEncoded;  //> Encoding signal(called after decoding)

    sage::Scope<sage::SubstanceConfig> _subConfig;

    std::thread _mainThread;  //> Main substance thread

    sage::Ref<sage::CamerasHandler> _camera = nullptr;      //> Camera base handler
    sage::Scope<sage::SubstanceState> _subState = nullptr;  //> Current Substance State

    ElapsedTimer timer;  //> fps counter timer
    ElapsedTimer tst;    //> fps counter timer

    uint fps;                      //> current fps value
    std::mutex mtx_;               //> Thread locker
    bool _isInited;                //> is substace inited;
    std::atomic<bool> _inProcess;  //> is substance enabled;
};

}  // namespace sage