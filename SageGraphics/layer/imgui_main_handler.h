#pragma once

#include <map>
#include <mutex>
#include <string>
#include <unordered_map>

#include "imgui_defintions.h"
#include "definitions/local_definitions.h"

#define IMGUI_IMPL_API
#include "../3rdParty/imgui/imgui_internal.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

// using MapSubstState = std::unordered_map<uint8_t, const sage::CameraState*>;
using MapSubstState = std::unordered_map<uint8_t, const sage::SubstanceState*>;

class ImgGuiMainHandler {
public:
    ImgGuiMainHandler() = default;
    ~ImgGuiMainHandler() = default;
    void mainHandler();
    void createMainWindow();
    void closeWindow();
    void firstRunInit(ImVec2& size);

    void updateSubstancePlot(const sage::SubstanceState& subst);

    MapSubstState* getSubstanceState();
    MapSubstState* getCameraInfo();
    

    bool isFirstLaunch_ = true;


private:
    void updateSettingsWindow();
    void updateMenuBar();
    void updateLogging();
    void updateViewPort();
    void updateManager();

private:
    bool isShowingDemo_ = false;  //< Show demo window
    bool isDockingEnabled_ = true;
    bool isFullScreenEnabled_ = true;
    bool isDockSpaceOpened_ = true;
    bool isShowingWinManager_ = true;
    char txtBoxCamUrl[256];

    ImGuiID dockspaceId_;
    std::mutex mtx_;

private:
    int readerType_ = 0;
    int ffmpegCaptureType_ = 0;
    int opencvCaptureType_ = 0;
    int textureSize_ = 0;
    int decoderType_ = 0;

private:
    ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

    MapSubstState cameraInfo;
    MapSubstState substanceState;

    std::map<int8_t, ImGuiCameraSettings> camSettings_;
};