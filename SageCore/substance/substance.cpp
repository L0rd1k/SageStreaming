#include "substance.h"

sage::Substance::Substance(short id, bool newOne)
        : _id(id), 
        _camera(nullptr), 
        _inProcess(false) {

    _subInfo = std::make_unique<SubstanceInfo>();
    _state = std::make_unique<CameraState>();
    if(!newOne) {
        initConfig();
    }
    // _isInited = initSubstance();
}

sage::Substance::~Substance() {
    if (_mainThread.joinable()) {
        _mainThread.join();
    }
    delete _camera;
    delete _decoder;
}

void sage::Substance::initConfig() {
    _camType = toCamType(sage::IniParser::inst().get("reader_type", _id));
    _decType = toDecType(sage::IniParser::inst().get("decoder_type", _id));
    _transportType = toFFmpegCapType(sage::IniParser::inst().get("ffmpegcap_type", _id));
    _cvcaptureType = toOpenCVCapType(sage::IniParser::inst().get("cvcap_type", _id));
    _url = sage::IniParser::inst().get("name", _id);
}

bool sage::Substance::initSubstance() {
    _camera = CamerasCreator::inst().createCamera(_camType, _url, _transportType, _cvcaptureType);
    if (!_camera) {
        return false;
    }
    _decoder = CamerasCreator::inst().createDecoder(_decType);
    if (!_decoder) {
        return false;
    }
    /** Substance info. **/
    _subInfo->camType = _camType;
    _subInfo->decType = _decType;
    _subInfo->id = _id;
    /** Camera state. **/
    _state->id = _id;
    _state->url = _url;
    _state->camType = _camType;
    _state->decType = _decType;
    connectCallbacks();
    timer.start();
    _isInited = true;
    return true;
}

short sage::Substance::getId() { return _id; }

bool sage::Substance::isEnabled() { return _inProcess; }

sage::CamerasHandler* sage::Substance::getCamera() { return _camera; }

sage::Decoder* sage::Substance::getDecoder() { return _decoder; }

void sage::Substance::connectCallbacks() {
    if (_camera) {
        callbacks.push_back(
            std::make_unique<void*>(_camera->sig_imageRecieved.connect(
                this, &sage::Substance::onImageReceived)));
    }
    if (_decoder) {
        callbacks.push_back(std::make_unique<void*>(
            sig_imageDecoded.connect(this, &sage::Substance::onImageDecode)));
    }
}

bool sage::Substance::enableSubstance() {
    if (_isInited) {
        if (!isEnabled()) {
            _inProcess = true;
            _mainThread = std::thread(&sage::Substance::mainSubstanceLoop, this);
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
    sig_sendSubstInfo.emit(*_subInfo);
    sig_sendSubstState.emit(*_state);
}

void sage::Substance::onImageReceived(const sage::swImage& img) {
    /** Test received fps from camera. **/

    // Log::trace("Received: ", _id);

    _subInfo->size = img->imgSize;
    _subInfo->duration = img->duration;
    _subInfo->format = img->imgFormat;
    if (timer.elapsedMs() > 1000) {
        _subInfo->fps = fps;
        sig_LogMsgSend.emit(std::to_string(_id) +
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
        _camType == sage::CamTypes::OPENCV) {
        decImg = img;
    } else {
        _decoder->decode(img, decImg);
    }
    _decoder->getQueue()->moveNext();
}

void sage::Substance::setCamReaderType(const sage::CamTypes type) { 
    _camType = type; 
}

void sage::Substance::setCamDecoderType(const sage::DecTypes type) {
    _decType = type;
}

void sage::Substance::setCamUrl(const std::string url) { 
    _url = url; 
}

void sage::Substance::setFFmpegCaptureType(const sage::RtspTransportType type) {
    _transportType = type;
}

void sage::Substance::setOpenCVCaptureType(const sage::CVCapType type) {
    _cvcaptureType = type;
}
