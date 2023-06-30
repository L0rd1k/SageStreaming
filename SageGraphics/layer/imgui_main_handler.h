#pragma once

#include <string>

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
    bool isShowingDemo_ = false;  //< Show demo window
    bool isDockingEnabled_ = true;
    bool isFullScreenEnabled_ = true;
    bool isDockSpaceOpened_ = true;
    bool isFirstLaunch_ = true;

    ImGuiID dockspaceId_;

private:
    ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
};