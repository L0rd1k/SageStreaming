#include "substance.h"

sage::Substance::Substance(short id, sage::CamTypes camType, sage::DecTypes decType)
    : _id(id),
      _camera(nullptr),
      _camType(camType),
      _decType(decType),
      _inProcess(false) {
    _subInfo = std::make_unique<SubstanceInfo>();
    _isInited = initSubstance(camType, decType);
    connectCallbacks();
    timer.start();
}

sage::Substance::~Substance() {
    if (_mainThread.joinable()) {
        _mainThread.join();
    }
    delete _camera;
}

bool sage::Substance::initSubstance(sage::CamTypes& camType, sage::DecTypes& decType) {
    _camera = CamerasCreator::inst().createCamera(_id, camType);
    if (!_camera)
        return false;
    _decoder = CamerasCreator::inst().createDecoder(decType);
    if (!_decoder)
        return false;

    /** Substance info. **/
    _subInfo->camType = camType;
    _subInfo->decType = decType;
    _subInfo->_id = _id;
    
    return true;
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
        onSubstanceInfoSend();
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
}

void sage::Substance::onImageReceived(const sage::swImage& img) {
    /** Test received fps from camera. **/
    _subInfo->_size = img->imgSize;
    _subInfo->duration = img->duration;
    _subInfo->format = img->imgFormat;
    if (timer.elapsedMs() > 1000) {
        _subInfo->_fps = fps;
        sig_LogMsgSend.emit(std::to_string(_id) + ":  Fps:" + std::to_string(fps) + "\n");
        fps = 0;
        timer.restart();
    } else {
        fps++;
    }

    sig_imageDecoded.emit(img);
}

void sage::Substance::onImageDecode(const sage::swImage& img) {
    sage::swImage& decImg = _decoder->getQueue()->next();
    /** @brief Add raw checking for opencv fwork **/
    if (img->imgFormat == sage::ImageFormat::RAW || _camType == sage::CamTypes::OPENCV) {
        decImg = img;
    } else {
        _decoder->decode(img, decImg);
    }
    _decoder->getQueue()->moveNext();
}
