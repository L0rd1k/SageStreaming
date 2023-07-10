#pragma once

#include <string>
#include <mutex>
#include "imgui_defintions.h"

#define IMGUI_IMPL_API
#include "../3rdParty/imgui/imgui_internal.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


class ImgGuiMainHandler {
public:
    ImgGuiMainHandler() = default;
    ~ImgGuiMainHandler() = default;
    void mainHandler();
    void createMainWindow();
    void closeWindow();
    void firstRunInit(ImVec2& size);
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
    bool isFirstLaunch_ = true;
    bool isShowingWinManager_ = true;

    ImGuiID dockspaceId_;

    std::mutex mtx_;

    int readerType_ = 0;
    int ffmpegCaptureType_ = 0;
    int opencvCaptureType_ = 0;
    int textureSize_ = 0;
    int decoderType_ = 0;

private:
    ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
};