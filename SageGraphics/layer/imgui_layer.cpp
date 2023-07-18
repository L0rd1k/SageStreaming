#include "imgui_layer.h"

#define IMGUI_IMPL_API
#include "../3rdParty/imgui/imgui_internal.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "window/window_painter_glfw.h"

// bool sage::GuiLayer::_winManager = true;
// bool sage::GuiLayer::first_time = true;
ImGuiLog sage::GuiLayer::log;

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
    dockLog();
}

ImgGuiMainHandler* sage::GuiLayer::getGuiHandler() {
    return &imguiHandler_;
}

void sage::GuiLayer::dockLog() {
    ImGui::Begin(loggingWinName.c_str());
    ImGui::End();
    log.Draw(loggingWinName.c_str());
}

void sage::GuiLayer::appendLog(const std::string& str) {
    log.AddLog(str.c_str());
}

void sage::GuiLayer::appendSubstInfo(const SubstanceState& subst) {
    std::lock_guard<std::mutex> _locker(_mtx);
    imguiHandler_.updateSubstancePlot(subst);
    imguiHandler_.getCameraInfo()->insert(std::make_pair(subst.id, &subst));

}

void sage::GuiLayer::appendSubstState(const SubstanceState& subst) {
    std::lock_guard<std::mutex> _locker(_mtx);
    imguiHandler_.getSubstanceState()->insert(std::make_pair(subst.id, &subst));
}


void sage::GuiLayer::setGuiWindow(GLFWwindow* win) {
    _winPtr = win;
}