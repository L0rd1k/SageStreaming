#include "substance.h"
sage::Substance::Substance(short id, bool newOne)
    : _camera(nullptr),
      _inProcess(false) {
    _subConfig = std::make_unique<sage::SubstanceConfig>(id);
    _subState = std::make_unique<sage::SubstanceState>();
    if (!newOne) {
        _subConfig->init();
        extractConfigToState();
    }
}

sage::Substance::~Substance() {
    _inProcess.store(false);
    if (_camera && _camera->isStreaming()) {
        _camera->stop();
    }
    if (_mainThread.joinable()) {
        _mainThread.join();
    }
    if (_decoder) {
        delete _decoder;
    }
    if (_encoder) {
        delete _encoder;
    }
}

bool sage::Substance::initSubstance() {
    _camera = CamerasCreator::inst().createCamera(
        getConfig()->getCamReaderType(),
        getConfig()->getCamUrl(),
        getConfig()->getFFmpegCaptureType(),
        getConfig()->getOpenCVCaptureType());

    if (!_camera) {
        return false;
    }
    _decoder = CamerasCreator::inst().createDecoder(getConfig()->getCamDecoderType());
    if (!getDecoder()) {
        Log::error("Decoder wasn't created", getConfig()->getId());
        return false;
    }

    // _encoder = CamerasCreator::inst().createEncoder(getConfig()->getCamEncoderType());
    // if (!_encoder) {
    //     Log::error("Encoder wasn't created", getConfig()->getId());
    //     return false;
    // }

    connectCallbacks();
    _isInited = true;
    return true;
}

void sage::Substance::extractConfigToState() {
    /** Substance info. **/
    if (_subState) {
        _subState->camType = getConfig()->getCamReaderType();
        _subState->decType = getConfig()->getCamDecoderType();
        _subState->id = getConfig()->getId();
        _subState->url = getConfig()->getCamUrl();
        _subState->isSubstEnabled = getConfig()->isSubstEnabled();
    }
}

void sage::Substance::updateConfig(const sage::SubstanceState& camState) {
    getConfig()->setCamReaderType(camState.camType);
    getConfig()->setCamDecoderType(camState.decType);
    getConfig()->setCamUrl(camState.url);
    getConfig()->setFFmpegCaptureType(camState.capTypeFFmpeg);
    getConfig()->setOpenCVCaptureType(camState.capTypeOpencv);
    getConfig()->setSubstEnabled(camState.isSubstEnabled);
    
    getConfig()->saveToConfigFile(camState);
}

void sage::Substance::connectCallbacks() {
    if (_camera) {
        connect(&_camera->sig_imageRecieved, this, &sage::Substance::onImageReceived);
    }
    if (_decoder) {
        connect(&sig_imageDecoded, this, &sage::Substance::onImageDecode);
    }
    if (_encoder) {
        connect(&sig_imageEncoded, this, &sage::Substance::onImageEncode);
    }
}

bool sage::Substance::enableSubstance() {
    if (_isInited) {
        if (!isEnabled()) {
            _inProcess.store(true);
            _mainThread = std::thread(&sage::Substance::mainSubstanceLoop, this);
            return true;
        }
    }
    return false;
}

bool sage::Substance::disableSubstance() {
    if (isEnabled()) {
        _inProcess.store(false);
        if (_mainThread.joinable()) {
            _mainThread.join();
        }
        if (_camera->isStreaming()) {
            _camera->stop();
            return true;
        }
    }
    return false;
}

void sage::Substance::startCameraStreaming() {
    if (_camera && !_camera->isStreaming()) {
        _camera->start();
    }
}

void sage::Substance::mainSubstanceLoop() {
    startCameraStreaming();
    onSubstanceInfoSend();
    while (_inProcess.load()) {
    }
}

const ImageQueue* sage::Substance::getImageQueue() {
    if (_camera) {
        auto imageFormat = _camera->getImageFormat();
        if (imageFormat == sage::ImageFormat::RAW) {
            return _camera->getQueue();
        }
    }
}

void sage::Substance::onSubstanceInfoSend() {
    sigSendSubstParams.emit(*_subState);
}

void sage::Substance::onImageReceived(const sage::swImage& img) {
    /** Test received fps from camera. **/
    _subState->size = img->imgSize;
    _subState->duration = img->duration;
    _subState->format = img->imgFormat;

    if (timer.elapsedMs() > 1000) {
        _subState->fps = fps;
#ifdef USE_IMGUI
        Log::trace((int)getConfig()->getId(), "- Fps:", (int)fps);
#endif
        fps = 0;
        timer.restart();
    } else {
        fps++;
    }
    sig_imageDecoded.emit(img);
}

void sage::Substance::onImageDecode(const sage::swImage& img) {
    sage::swImage& decImg = _decoder->getQueue()->next();
    /** @brief Added raw checking for opencv fwork **/
    if (img->imgFormat == sage::ImageFormat::RAW ||
        getConfig()->getCamReaderType() == sage::CamTypes::OPENCV) {
        decImg = img;
    } else {
        _decoder->decode(img, decImg);
    }
    sig_imageEncoded.emit(decImg);
    _decoder->getQueue()->moveNext();
}

void sage::Substance::onImageEncode(const sage::swImage& img) {
    sage::swImage& encImg = _encoder->getQueue()->next();
    if (img->imgSize.isValid()) {
        _encoder->encode(img, encImg);
    }
    _encoder->getQueue()->moveNext();
}

bool sage::Substance::isEnabled() {
    std::lock_guard<std::mutex> lock(mtx_);
    return _inProcess.load();
}

std::shared_ptr<sage::CamerasHandler> sage::Substance::getCamera() {
    return _camera;
}

sage::Decoder* sage::Substance::getDecoder() {
    return _decoder;
}

sage::Encoder* sage::Substance::getEncoder() {
    return _encoder;
}

sage::Scope<sage::SubstanceConfig>& sage::Substance::getConfig() {
    return _subConfig;
}