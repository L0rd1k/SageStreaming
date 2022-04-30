#include "UMSubstance.h"

UMSubstance::UMSubstance(short id) :
_id(id),
_camera(nullptr),
_inProcess(false) {
    initSubstance();
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

void UMSubstance::mainSubstanceLoop() {
    if(!_camera->isStreaming()) {
        _camera->start();
    }   

    while(_isLoopOn) {
        
    }
}

const ImageQueue* UMSubstance::getImageQueue() {
    auto imageFormat = _camera->getImageFormat();
    if(imageFormat == img::ImageFormat::RAW) {
        return _camera->getQueue();
    }
}