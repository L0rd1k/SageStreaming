#include "imgui_main_window.h"

#define IMGUI_IMPL_API
#include "imgui_internal.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "window/window_painter_glfw.h"

void sage::GuiMainWindow::init() {
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

void sage::GuiMainWindow::detach() {
    ImGui::DestroyContext();
    // ImGui_ImplGlfw_Shutdown();
    // ImGui_ImplOpenGL3_Shutdown();
}

void sage::GuiMainWindow::beginDraw() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    // Recreate fonts after adding new texture.
    if(getGuiHandler()->isFirstLaunch_) {
        ImGui_ImplOpenGL3_CreateFontsTexture();
    }
    ImGui::NewFrame();
}

void sage::GuiMainWindow::initFontsTexture() {
    ImGui_ImplOpenGL3_CreateFontsTexture();
}

void sage::GuiMainWindow::endDraw() {
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

void sage::GuiMainWindow::processDraw() {
    imguiHandler_.mainHandler();
    updateLog();
}

ImgGuiMainHandler* sage::GuiMainWindow::getGuiHandler() {
    return &imguiHandler_;
}

void sage::GuiMainWindow::updateLog() {
    log.draw(loggingWinName.c_str());
}

void sage::GuiMainWindow::appendLog(const std::string& str, const sage::LogLevels& lvl) {
    log.addLog(str, lvl);
}

void sage::GuiMainWindow::appendSubstState(const SubstanceState& subst) {
    std::lock_guard<std::mutex> _locker(_mtx);
    imguiHandler_.getSubstanceState()->insert(std::make_pair(subst.id, &subst));
    imguiHandler_.camSettings.insert({subst.id, ImGuiCameraSettings()});
    imguiHandler_.camSettings[subst.id].setCameraActive(subst.isSubstEnabled);
}

void sage::GuiMainWindow::setGuiWindow(GLFWwindow* win) {
    _winPtr = win;
}