#include "UMSubstance.h"

UMSubstance::UMSubstance(short id) :
_id(id),
_camera(nullptr),
_inProcess(false) {
    initSubstance();
    connectCallbacks();
}

UMSubstance::~UMSubstance() {
    if(_mainThread.joinable()) {
        _mainThread.join();
    }
    delete _camera;
}

bool UMSubstance::initSubstance() {
    _camera = CamerasCreator::inst().createCamera(_id, cam::CamTypes::FFMPEG);
    if(_camera)
        return false;
    return true;
}

bool UMSubstance::isEnabled() {
    return _inProcess;
}

CamerasHandler* UMSubstance::getCamera() {
    return _camera;
} 

void UMSubstance::connectCallbacks() {
    if(_camera) {
        callbacks.push_back(std::make_unique<void*>(_camera->sig_imageRecieved.connect(this, &UMSubstance::onImageReceived)));
    }
}

bool UMSubstance::enableSubstance() {
    if(!isEnabled()) {
        _inProcess = true;
        _mainThread = std::thread(&UMSubstance::mainSubstanceLoop, this);
    }
    return false;
}

bool UMSubstance::disableSubstance() {
    if(isEnabled()) {
        _inProcess = false;
        if(_camera->isStreaming()) {
            _camera->stop();
            return true;
        }
    }
    return false;
}

void UMSubstance::startCameraStreaming() {
    if(!_camera->isStreaming()) {
        _camera->start();
    }    
}

void UMSubstance::mainSubstanceLoop() {
    startCameraStreaming();

    while(_inProcess) {
        Log() << "Main loop: " << _id << " " << std::this_thread::get_id();
        usleep(5000);
    }
}

const ImageQueue* UMSubstance::getImageQueue() {
    auto imageFormat = _camera->getImageFormat();
    if(imageFormat == img::ImageFormat::RAW) {
        return _camera->getQueue();
    }
}

void UMSubstance::onImageReceived(const img::swImage& img) {
    Log() << img->imgSize.height() << "x" << img->imgSize.width();
}