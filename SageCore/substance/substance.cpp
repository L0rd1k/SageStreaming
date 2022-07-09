#include "substance.h"

sage::Substance::Substance(short id)
    : _id(id),
      _camera(nullptr),
      _inProcess(false) {
    initSubstance();
    connectCallbacks();
}

sage::Substance::~Substance() {
    if (_mainThread.joinable()) {
        _mainThread.join();
    }
    delete _camera;
}

bool sage::Substance::initSubstance() {
    _camera = CamerasCreator::inst().createCamera(_id, cam::CamTypes::FFMPEG);
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
        // usleep(5000);
    }
}

const ImageQueue* sage::Substance::getImageQueue() {
    auto imageFormat = _camera->getImageFormat();
    if (imageFormat == img::ImageFormat::RAW) {
        return _camera->getQueue();
    }
}

void sage::Substance::onImageReceived(const img::swImage& img) {
    // Log::debug(img->imgSize.height(), "x", img->imgSize.width());
    img::swImage& decImg = _decoder->getQueue()->next();
    _decoder->decode(img, decImg);
    _decoder->getQueue()->moveNext();
    // cv::imwrite("/home/ilya/img_ffmpeg.png", cv_toMat(decImg));

}