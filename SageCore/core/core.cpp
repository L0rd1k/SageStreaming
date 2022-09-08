#include "core.h"

void sage::Core::createSubstances(int count = 0) {
    for (int id = 0; id < count; id++) {
        auto camType = toCamType(sage::IniParser::inst().get("reader_type", id));
        auto decType = toDecType(sage::IniParser::inst().get("decoder_type", id));
        if (camType != sage::CamTypes::Undefined && decType != sage::DecTypes::Undefined) {
            _substns.push_back(std::make_shared<sage::Substance>(id, camType, decType));
            _substns.back()->enableSubstance();
        }
    }
}

void sage::Core::createSingleSubstance(const sage::CameraState& camState) {
    uint8_t id_size = _substns.size();
    _substns.push_back(std::make_shared<sage::Substance>(id_size, camState.camType, camState.decType, camState.url,
                                                         camState.ffmpegcapType, camState.cvcapType, true));
    Log::critical(_substns.back()->getId());
    if (_substns.back()->_decoder) {
        _pic->createTexture();
        _pic->setDataBuffer(id_size, _substns.back()->_decoder->getQueue());
    }
    _substns.back()->enableSubstance();
#ifdef USE_IMGUI
    _window->getGuiLayer()->first_time = true;
    global_callbacks.push_back(
        std::make_unique<void*>(_substns.back()->sig_sendSubstInfo.connect(_window->getGuiLayer(),
                                                                        &sage::GuiLayer::appendSubstInfo)));
    global_callbacks.push_back(
        std::make_unique<void*>(_substns.back()->sig_sendSubstState.connect(_window->getGuiLayer(),
                                                                         &sage::GuiLayer::appendSubstState)));
#endif
}

void sage::Core::createWindow(int argc, char** argv) {
#ifdef USE_GLUT
    _window = std::make_unique<WindowPainterGlut>();
#elif USE_GLFW
    _window = std::make_unique<WindowPainterGLFW>();
#endif
    _window->createWindow(argc, argv, sage::Size<int>(1024, 768));
    _pic = std::make_shared<PicturePainter>(_substns.size());
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
