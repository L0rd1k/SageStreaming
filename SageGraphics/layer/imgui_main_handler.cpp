#include "imgui_main_handler.h"

#include "window/window_painter_glfw.h"

void ImgGuiMainHandler::mainHandler() {
    // ImGui::ShowDemoWindow(&isShowingDemo_);
    if (isDockingEnabled_) {
        createMainWindow();
        ImGuiIO& io = ImGui::GetIO();
        // Check if docking is available.
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
            ImVec2 availableSize = ImGui::GetContentRegionAvail();
            dockspaceId_ = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspaceId_, ImVec2(0.0f, 0.0f), dockspace_flags);
            if (isFirstLaunch_) {
                firstRunInit(availableSize);
                isFirstLaunch_ = false;
            }
        }





        closeWindow();
    }
}

void ImgGuiMainHandler::firstRunInit(ImVec2& size) {
    ImVec2 availableSize = ImGui::GetContentRegionAvail();
    ImGui::DockBuilderRemoveNode(dockspaceId_);  // clear any previous layout
    ImGui::DockBuilderAddNode(dockspaceId_, ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_DockSpace);
    ImGui::DockBuilderSetNodeSize(dockspaceId_, size);

    // Create Viewport Windows
    // for (int i = 0; i <= WindowPainterGLFW::inst().getPicturePainter()->getTexturesCount(); i++) {
    //     std::string name = "Viewport" + std::to_string(i);
    //     ImGui::DockBuilderDockWindow(name.c_str(), dockspaceId_);
    //     ImGui::DockBuilderFinish(dockspaceId_);
    // }


    ImGui::DockBuilderFinish(dockspaceId_);
}

void ImgGuiMainHandler::createMainWindow() {
    if (isFullScreenEnabled_) {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        window_flags |= ImGuiWindowFlags_NoTitleBar |
                        ImGuiWindowFlags_NoCollapse |
                        ImGuiWindowFlags_NoResize |
                        ImGuiWindowFlags_NoMove |
                        ImGuiWindowFlags_NoBringToFrontOnFocus |
                        ImGuiWindowFlags_NoNavFocus;
    }
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) {
        window_flags |= ImGuiWindowFlags_NoBackground;
    }
    ImGui::Begin("DockSpace Demo", &isDockSpaceOpened_, window_flags);
    if (isFullScreenEnabled_) {
        ImGui::PopStyleVar(3);
    }
}

void ImgGuiMainHandler::closeWindow() {
    ImGui::End();
}
