#include "core.h"

void sage::Core::createSubstances(uint8_t count = 0) {
    for (uint8_t id = 0; id < count; id++) {
        //> Select Reader type.
        auto camType = toCamType(sage::IniParser::inst().get("reader_type", id));
        //> Select Decoder type.
        auto decType = toDecType(sage::IniParser::inst().get("decoder_type", id));
        //> Create substance if its parameters defined.
        if (camType != sage::CamTypes::Undefined && decType != sage::DecTypes::Undefined) {
            _substns[id] = std::make_shared<sage::Substance>(id, camType, decType);
        }
    }
}

void sage::Core::runSubstances() {
    Log::info(_substns.size());
    for(uint8_t id = 0; id < _substns.size(); id++) {
        //> If substance not empty, launch it.
        if(_substns[id]) {
            _substns[id]->enableSubstance();
        }
    }
}

void sage::Core::createSingleSubstance(const sage::CameraState& camState) {
    //> Check substance list size.
    uint8_t new_id = _substns.size();
    //> Create new substance.
    _substns[new_id] = std::make_shared<sage::Substance>(new_id,
        camState.camType, 
        camState.decType, 
        camState.url,
        camState.ffmpegcapType, 
        camState.cvcapType, 
        true);
    Log::critical(_substns[new_id]->getId());
    //> If decoder created, create texture and assign image queue to it.
    if(_substns[new_id]->getDecoder()) {
        _pic->createTexture();
        _pic->setDataBuffer(new_id, _substns[new_id]->getDecoder()->getQueue());
    }
    _substns[new_id]->enableSubstance();  
    _window->getGuiLayer()->first_time = true;

#ifdef USE_IMGUI
    global_callbacks.push_back(
        std::make_unique<void*>(_substns[new_id]->sig_sendSubstInfo.connect(_window->getGuiLayer(),
                                                                        &sage::GuiLayer::appendSubstInfo)));
    global_callbacks.push_back(
        std::make_unique<void*>(_substns[new_id]->sig_sendSubstState.connect(_window->getGuiLayer(),
                                                                         &sage::GuiLayer::appendSubstState)));
#endif
}

void sage::Core::createWindow(int argc, char** argv) {
#ifdef USE_GLUT
    _window = std::make_unique<WindowPainterGlut>();
#elif USE_GLFW
    _window = std::make_unique<WindowPainterGLFW>();
#endif
    //> Create window context.
    _window->createWindow(argc, argv, sage::Size<int>(1024, 768));
    //> Create textures in painter for every substance. 
    _pic = std::make_shared<PicturePainter>(_substns.size());
    //> Set current picture painter.
    _window->setPicturePainter(_pic);
}


void sage::Core::enableWindow() {
    _window->run();
}

#ifdef USE_IMGUI
void sage::Core::sendLog(const std::string& str) {
    _window->getGuiLayer()->appendLog(str);
}
#endif

void sage::Core::connectCamBufToWindow() {
    for (int id = 0; id < _substns.size(); id++) {
        if (_substns[id]->_decoder) {
            _pic->setDataBuffer(id, _substns[id]->_decoder->getQueue());
        }
    }
}

void sage::Core::enableCallbacks() {
#ifdef USE_IMGUI
    global_callbacks.push_back(
        std::make_unique<void*>(sig_LogMsgSend.connect(this, &sage::Core::sendLog)));
    global_callbacks.push_back(
        std::make_unique<void*>(sig_sendCameraState.connect(this, &sage::Core::createSingleSubstance)));

    for (int id = 0; id < _substns.size(); id++) {
        global_callbacks.push_back(
            std::make_unique<void*>(_substns[id]->sig_sendSubstInfo.connect(_window->getGuiLayer(),
                                                                            &sage::GuiLayer::appendSubstInfo)));
        global_callbacks.push_back(
            std::make_unique<void*>(_substns[id]->sig_sendSubstState.connect(_window->getGuiLayer(),
                                                                             &sage::GuiLayer::appendSubstState)));
    }
#endif
}
