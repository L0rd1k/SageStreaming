#include "substance.h"
sage::Substance::Substance(short id, bool newOne)
    : _camera(nullptr), _inProcess(false) {
    _subConfig = std::make_unique<sage::SubstanceConfig>(id);
    _subState = std::make_unique<SubstanceState>();
    _camState = std::make_unique<CameraState>();
    if (!newOne) {
        _subConfig->init();
    }
}

sage::Substance::~Substance() {
    if (_mainThread.joinable()) {
        _mainThread.join();
    }
    delete _camera;
    delete _decoder;
    delete _encoder;
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
        Log::critical("Decoder wasn't created", getConfig()->getId());
        return false;
    }
    _encoder = CamerasCreator::inst().createEncoder(getConfig()->getCamEncoderType());
    if (!_encoder) {
        Log::critical("Encoder wasn't created", getConfig()->getId());
        return false;
    }

    /** Substance info. **/
    _subState->camType = getConfig()->getCamReaderType();
    _subState->decType = getConfig()->getCamDecoderType();
    _subState->id = getConfig()->getId();

    /** Camera state. **/
    _camState->id = getConfig()->getId();
    _camState->url = getConfig()->getCamUrl();
    _camState->camType = getConfig()->getCamReaderType();
    _camState->decType = getConfig()->getCamDecoderType();

    connectCallbacks();
    _isInited = true;
    return true;
}

void sage::Substance::connectCallbacks() {
    if (_camera) {
        callbacks.push_back(std::make_unique<void*>(
            _camera->sig_imageRecieved.connect(this, &sage::Substance::onImageReceived)));
    }
    if (_decoder) {
        callbacks.push_back(std::make_unique<void*>(
            sig_imageDecoded.connect(this, &sage::Substance::onImageDecode)));
    }
    if (_encoder) {
        callbacks.push_back(std::make_unique<void*>(
            sig_imageEncoded.connect(this, &sage::Substance::onImageEncode)));
    }
}

bool sage::Substance::enableSubstance() {
    if (_isInited) {
        if (!isEnabled()) {
            _inProcess = true;
            _mainThread =
                std::thread(&sage::Substance::mainSubstanceLoop, this);
            return true;
        }
    }
    return false;
}

bool sage::Substance::disableSubstance() {
    if (isEnabled()) {
        _inProcess = false;
        if (_camera->isStreaming()) {
            _camera->stop();
            return true;
        }
    }
    return false;
}

void sage::Substance::startCameraStreaming() {
    if (!_camera->isStreaming()) {
        _camera->start();
    }
}

void sage::Substance::mainSubstanceLoop() {
    startCameraStreaming();
    while (_inProcess) {
    }
}

const ImageQueue* sage::Substance::getImageQueue() {
    auto imageFormat = _camera->getImageFormat();
    if (imageFormat == sage::ImageFormat::RAW) {
        return _camera->getQueue();
    }
}

void sage::Substance::onSubstanceInfoSend() {
    sig_sendSubstInfo.emit(*_subState);
    sig_sendSubstState.emit(*_camState);
}

void sage::Substance::onImageReceived(const sage::swImage& img) {
    /** Test received fps from camera. **/
    _subState->size = img->imgSize;
    _subState->duration = img->duration;
    _subState->format = img->imgFormat;

    if (timer.elapsedMs() > 1000) {
        _subState->fps = fps;
        sig_LogMsgSend.emit(std::to_string(getConfig()->getId()) +
                            ":  Fps:" + std::to_string(fps) + "\n");
        fps = 0;
        timer.restart();
        onSubstanceInfoSend();  // Send cam info once per second;
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
    if(img->imgSize.isValid()) {
        _encoder->encode(img, encImg);
    }
    _encoder->getQueue()->moveNext();
}

bool sage::Substance::isEnabled() {
    return _inProcess;
}

sage::CamerasHandler* sage::Substance::getCamera() {
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