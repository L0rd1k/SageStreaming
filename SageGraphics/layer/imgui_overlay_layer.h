#pragma once 

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#define IMGUI_IMPL_API


static void applicationInfo(bool* p_open) {
    ImGuiIO& io = ImGui::GetIO();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    const float PAD = 5.0f;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 window_pos, window_pos_pivot;
    window_pos.x = (viewport->WorkPos.x + viewport->WorkSize.x - PAD);
    window_pos.y = (viewport->WorkPos.y + viewport->WorkSize.y - PAD);
    window_pos_pivot.x = 1.0f;
    window_pos_pivot.y = 1.0f;
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
    ImGui::SetNextWindowViewport(viewport->ID);
    window_flags |= ImGuiWindowFlags_NoMove;
    ImGui::SetNextWindowBgAlpha(0.35f);
    if (ImGui::Begin("Example: Simple overlay", p_open, window_flags)) {
        ImGui::Text("Window Info\n");
        ImGui::Separator();
    }
    ImGui::End();
}