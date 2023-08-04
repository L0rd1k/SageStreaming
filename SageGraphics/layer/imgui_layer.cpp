#include "imgui_layer.h"

#define IMGUI_IMPL_API
#include "../3rdParty/imgui/imgui_internal.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "window/window_painter_glfw.h"

void sage::GuiLayer::init() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    ImGui::StyleColorsClassic();
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
    ImGui_ImplGlfw_InitForOpenGL(_winPtr, true);
    ImGui_ImplOpenGL3_Init("#version 300 es");
}

void sage::GuiLayer::detach() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void sage::GuiLayer::beginDraw() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    // Recreate fonts after adding new texture.
    if(getGuiHandler()->isFirstLaunch_) {
        ImGui_ImplOpenGL3_CreateFontsTexture();
    }
    ImGui::NewFrame();
}

void sage::GuiLayer::initFontsTexture() {
    ImGui_ImplOpenGL3_CreateFontsTexture();
}

void sage::GuiLayer::endDraw() {
    ImGuiIO& io = ImGui::GetIO();
    auto size = WindowPainterGLFW::inst()._winSize;
    io.DisplaySize = ImVec2(size.width(), size.height());
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(context);
    }
}

void sage::GuiLayer::processDraw() {
    imguiHandler_.mainHandler();
    updateLog();
}

ImgGuiMainHandler* sage::GuiLayer::getGuiHandler() {
    return &imguiHandler_;
}

void sage::GuiLayer::updateLog() {
    log.draw(loggingWinName.c_str());
}

void sage::GuiLayer::appendLog(const std::string& str, const sage::LogLevels& lvl) {
    log.addLog(str, lvl);
}

void sage::GuiLayer::appendSubstState(const SubstanceState& subst) {
    std::lock_guard<std::mutex> _locker(_mtx);
    imguiHandler_.getSubstanceState()->insert(std::make_pair(subst.id, &subst));
    imguiHandler_.camSettings.insert({subst.id, ImGuiCameraSettings()});
    imguiHandler_.camSettings[subst.id].setCameraActive(subst.isSubstEnabled);
}

void sage::GuiLayer::setGuiWindow(GLFWwindow* win) {
    _winPtr = win;
}