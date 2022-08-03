#include "substance.h"

sage::Substance::Substance(short id)
    : _id(id),
      _camera(nullptr),
      _inProcess(false) {
    initSubstance();
    connectCallbacks();
    timer.start();
}

sage::Substance::~Substance() {
    if (_mainThread.joinable()) {
        _mainThread.join();
    }
    delete _camera;
}

bool sage::Substance::initSubstance() {
    _camera = CamerasCreator::inst().createCamera(_id, sage::CamTypes::FFMPEG);
    if (!_camera)
        return false;
    _decoder = CamerasCreator::inst().createDecoder(sage::DecTypes::FFMPEG);
    if (!_decoder)
        return false;

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
    // if (_decoder) {
    //     callbacks.push_back(
    //         std::make_shared<void*>(sig_imageDecoded.connect(this, &sage::Substance));
    //     )
    // }

}

bool sage::Substance::enableSubstance() {
    if (!isEnabled()) {
        _inProcess = true;
        _mainThread = std::thread(&sage::Substance::mainSubstanceLoop, this);
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

void sage::Substance::onImageReceived(const sage::swImage& img) {
    sage::swImage& decImg = _decoder->getQueue()->next();

    /** Test received fps from camera. **/
    if(timer.elapsedMs() > 1000) {
        Log::trace("Fps:", fps, " --> ", std::this_thread::get_id());
        fps = 0;
        timer.restart();
    } else {
        fps++;
    }

    if(img->imgFormat == sage::ImageFormat::RAW) {
        decImg = img;
    } else {
        _decoder->decode(img, decImg);
    }


    _decoder->getQueue()->moveNext();
}