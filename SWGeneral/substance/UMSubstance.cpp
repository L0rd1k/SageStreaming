#include "UMSubstance.h"

UMSubstance::UMSubstance(short id) :
_id(id),
_camera(nullptr),
_inProcess(false) {
    initSubstance();
}

UMSubstance::~UMSubstance() {
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

bool UMSubstance::enableSubstance() {
    if(!isEnabled()) {
        _inProcess = true;
        if(!_camera->isStreaming()) {
            Log() << "Start: " << _id;
            if(_camera->start()) {
                return true;
            }
        }
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