#pragma once

#include <atomic>
#include <map>
#include <mutex>
#include <string>
#include <memory>
#include <unordered_map>

#include "definitions/local_definitions.h"
#include "imgui_defintions.h"
#include "layer/node/imgui_video_node_editor.h"

#define IMGUI_IMPL_API
#include "imgui_internal.h"

#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui_overlay_layer.h"
#include "imgui_node/imgui_node_editor.h"

#include "IconsMaterialDesign.h"
#include "IconsFontAwesome4.h" 
using MapSubstState = std::map<uint8_t, const sage::SubstanceState*>;

class ImgGuiMainHandler {
public:
    ImgGuiMainHandler() = default;
    ~ImgGuiMainHandler() = default;
    void mainHandler();
    void mainHandler2();
    void createMainWindow();
    void closeWindow();
    void firstRunInit(ImVec2& size);

    void updateSubstancePlot(const sage::SubstanceState& subst);

    MapSubstState* getSubstanceState();
    // MapSubstState* getCameraInfo();

    bool isFirstLaunch_ = true;

    ccflow::Signal<const sage::SubstanceState&> sig_createCamera;
    
    ccflow::Signal<const uint8_t&> sig_removeCamera;
    ccflow::Signal<const uint8_t&, bool*> sig_activateCamera;
    ccflow::Signal<> sig_stopSubstances;
    ccflow::Signal<> sig_runSubstances;

    std::map<int8_t, ImGuiCameraSettings> camSettings;

private:

    std::unique_ptr<sage::ImGuiVideoNodeEditor> _videoNodeEditor;

    void sideBarMenu();


    void updateSettingsWindow();
    void updateMenuBar();
    void updateLogging();
    void updateViewPort();
    void updateManager();

private:
    bool isShowingDemo_ = true;  //< Show demo window
    bool isDockingEnabled_ = true;
    bool isFullScreenEnabled_ = true;
    bool isDockSpaceOpened_ = true;
    bool isShowingWinManager_ = false;

    bool isSplitDisplay_ = true;
    std::atomic<bool> isSubstnsLaunched_ = true;

    char txtBoxCamUrl[256];

    ImGuiID dockspaceId_;
    std::mutex mtx_;

private:
    int readerType_ = 0;
    int ffmpegCaptureType_ = 0;
    int opencvCaptureType_ = 0;
    int textureSize_ = 0;
    int decoderType_ = 0;
    bool testCheckBox;  

    int selectedItem = 0;

private:
    ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    ImGuiWindowFlags window_flags; // = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

    MapSubstState substanceState;
};