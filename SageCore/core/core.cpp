#include "core.h"

void sage::Core::createSubstances() {
    int count = sage::IniParser::inst().get(CFG_SUBSTANCE_COUNT);
    for (uint8_t id = 0; id < count; id++) {
        _activeSubstns[id] = std::move(std::make_unique<sage::Substance>(id));
        _activeSubstns[id]->initSubstance();
        if (_activeSubstns[id]->getConfig()->isSubstEnabled()) {
            activeSubstSize++;
        }
    }
}

void sage::Core::runSubstances() {
    for (const auto& subst : _activeSubstns) {
        if (subst.second->getConfig()->isSubstEnabled()) {
            if (subst.second->enableSubstance()) {
                Log::info("Run substance:", (int)subst.first);
            }
        } else {
            subst.second->onSubstanceInfoSend();
        }
    }
}

void sage::Core::stopSubstances() {
    for (const auto& subst : _activeSubstns) {
        Log::info("Stop substance:", (int)subst.first);
        subst.second->disableSubstance();
    }
}

void sage::Core::createSingleSubstance(const sage::SubstanceState& camState) {
    uint8_t new_id = sage::IniParser::inst().get(CFG_SUBSTANCE_COUNT);
    Log::info("Create new substance: ", (int)new_id);
    _activeSubstns[new_id] = std::make_unique<sage::Substance>(new_id, true);
    // Extract substance config data
    _activeSubstns[new_id]->updateConfig(camState);
    _activeSubstns[new_id]->extractConfigToState();
    _activeSubstns[new_id]->initSubstance();
    if (_activeSubstns[new_id]->getDecoder()) {
        _pic->createTexture();
        _pic->setDataBuffer(_pic->getTexturesCount() - 1, _activeSubstns[new_id]->getDecoder()->getQueue());
    }
#ifdef USE_IMGUI
    connect(&_activeSubstns[new_id]->sigSendSubstParams, _window->getGuiLayer(), &sage::GuiLayer::appendSubstState);
    _activeSubstns[new_id]->onSubstanceInfoSend();
    _window->getGuiLayer()->getGuiHandler()->isFirstLaunch_ = true;
#endif
}

void sage::Core::activateSubstance(const uint8_t& id, bool* isActive) {
    if(*isActive) {
        std::cout << (int)id << " - Active: " << *isActive << std::endl; 
    } else {
        std::cout << (int)id << " - Not Active: " << *isActive << std::endl; 
    }

}

void sage::Core::removeSingleSubstance(const uint8_t& removeId) {
    std::lock_guard<std::mutex> lock(mtx_);
    uint8_t id = removeId;
    bool onBootEnabled = _activeSubstns[id]->getConfig()->isSubstEnabled();
    _activeSubstns[id]->disableSubstance();  // Stop substance;
    if(onBootEnabled) {
        _pic->removeTexture(id);                 // Remove texture
    }
    _activeSubstns.erase(id);                // Remove substance
    if(onBootEnabled) {
        _pic->allocateTextures(_activeSubstns.size());
        _pic->reinitTextures();
        connectCamBufToWindow();
    }
    sage::IniParser::inst().set(CFG_SUBSTANCE_COUNT, sage::IniParser::inst().get(CFG_SUBSTANCE_COUNT) - 1);
    sage::IniParser::inst().remove(removeId, _activeSubstns.size());
    Log::info("Substance removed:", (int)id);
}

void sage::Core::createWindow(int argc, char** argv) {
#ifdef USE_GLUT
    _window = std::make_unique<WindowPainterGlut>();
#elif USE_GLFW
    _window = std::make_unique<WindowPainterGLFW>();
#endif
    _window->createWindow(argc, argv, sage::Size<int>(1920, 1080));  //> Create window context.
    _pic = std::make_shared<PicturePainter>(activeSubstSize);        //> Create textures in painter for every substance.
    _window->setPicturePainter(_pic);                                //> Set current picture painter.
}

void sage::Core::enableWindow() {
    _window->run();
}

#ifdef USE_IMGUI
void sage::Core::sendLog(const std::string& str, const sage::LogLevels& lvl) {
    _window->getGuiLayer()->appendLog(str, lvl);
}
#endif

void sage::Core::connectCamBufToWindow() {
    int counter = 0;
    for (const auto& subst : _activeSubstns) {
        if (subst.second->getConfig()->isSubstEnabled()) {
            if (subst.second->getDecoder()) {
                _pic->setDataBuffer(counter, subst.second->getDecoder()->getQueue());
                ++counter;
            }
        }
    }
}

void sage::Core::enableCallbacks() {
#ifdef USE_IMGUI
    connect(&sigLogSend, this, &sage::Core::sendLog);
    connect(&_window->getGuiLayer()->getGuiHandler()->sig_createCamera, this, &sage::Core::createSingleSubstance);
    connect(&_window->getGuiLayer()->getGuiHandler()->sig_removeCamera, this, &sage::Core::removeSingleSubstance);
    connect(&_window->getGuiLayer()->getGuiHandler()->sig_activateCamera, this, &sage::Core::activateSubstance);


    connect(&_window->getGuiLayer()->getGuiHandler()->sig_stopSubstances, this, &sage::Core::stopSubstances);
    connect(&_window->getGuiLayer()->getGuiHandler()->sig_runSubstances, this, &sage::Core::runSubstances);
    for (const auto& subst : _activeSubstns) {
        connect(&subst.second->sigSendSubstParams, _window->getGuiLayer(), &sage::GuiLayer::appendSubstState);
    }
#endif
}
