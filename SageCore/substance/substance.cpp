#include "substance.h"

sage::Substance::Substance(short id, sage::CamTypes camType, sage::DecTypes decType, std::string url,
                           sage::RtspTransportType transportType, sage::CVCapType cvcaptureType, bool newOne)
    : _id(id),
      _camera(nullptr),
      _camType(camType),
      _decType(decType),
      _inProcess(false) {
    _subInfo = std::make_unique<SubstanceInfo>();
    _state = std::make_unique<CameraState>();
    _isInited = initSubstance(url, _camType, _decType, transportType, cvcaptureType, newOne);
    connectCallbacks();
    timer.start();
}

sage::Substance::~Substance() {
    if (_mainThread.joinable()) {
        _mainThread.join();
    }
    delete _camera;
    delete _decoder;
}

bool sage::Substance::initSubstance(std::string url, sage::CamTypes& camType, sage::DecTypes& decType,
                                    sage::RtspTransportType transportType, sage::CVCapType cvcaptureType, bool newOne) {
    if(!newOne) {  
        url = sage::IniParser::inst().get("name", _id);                                  
        transportType = toFFmpegCapType(sage::IniParser::inst().get("ffmpegcap_type", _id));
        cvcaptureType = toOpenCVCapType(sage::IniParser::inst().get("cvcap_type", _id));
    }
    _camera = CamerasCreator::inst().createCamera(camType, url, transportType, cvcaptureType);
    if (!_camera)
        return false;
    _decoder = CamerasCreator::inst().createDecoder(decType);
    if (!_decoder)
        return false;

    /** Substance info. **/
    _subInfo->camType = camType;
    _subInfo->decType = decType;
    _subInfo->id = _id;

    /** Camera state. **/
    _state->id = _id;
    _state->url = url;
    _state->camType = camType;
    _state->decType = decType;

    return true;
}

short sage::Substance::getId() {
    return _id;
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

void sage::Substance::connectCallbacks() {
    if (_camera) {
        callbacks.push_back(
            std::make_unique<void*>(_camera->sig_imageRecieved.connect(this, &sage::Substance::onImageReceived)));
    }
    if (_decoder) {
        callbacks.push_back(
            std::make_unique<void*>(sig_imageDecoded.connect(this, &sage::Substance::onImageDecode)));
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
        // Log() << "Main loop: " << _id << " " << std::this_thread::get_id();
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
        sig_LogMsgSend.emit(std::to_string(_id) + ":  Fps:" + std::to_string(fps) + "\n");
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
    if (img->imgFormat == sage::ImageFormat::RAW || _camType == sage::CamTypes::OPENCV) {
        decImg = img;
    } else {
        _decoder->decode(img, decImg);
    }
    _decoder->getQueue()->moveNext();
}
