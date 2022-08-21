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

    ImGui::StyleColorsDark();
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
    ImGui::NewFrame();
}

void sage::GuiLayer::endDraw() {
    ImGuiIO& io = ImGui::GetIO();
    auto size = WindowPainterGLFW::inst()._winSize;
    io.DisplaySize = ImVec2(size.width(), size.height());
    io.DisplaySize = ImVec2(0, 0);
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
    // static bool isShow = true;
    // ImGui::ShowDemoWindow(&isShow);

    static bool dockingEnabled = true;
    if (dockingEnabled) {
        static bool dockspaceOpen = true;
        static bool opt_fullscreen_persistant = true;
        bool opt_fullscreen = opt_fullscreen_persistant;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen) {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
        ImGui::PopStyleVar();
        if (opt_fullscreen)
            ImGui::PopStyleVar(2);
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
            ImVec2 availableSize = ImGui::GetContentRegionAvail();
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

            static auto first_time = true;
            if (first_time) {
                first_time = false;
                ImGui::DockBuilderRemoveNode(dockspace_id);  // clear any previous layout
                ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_DockSpace);
                ImGui::DockBuilderSetNodeSize(dockspace_id, availableSize);

                /** Settings window. **/
                ImGuiID dockSettings = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.2f, nullptr, &dockspace_id);
                ImGui::DockBuilderDockWindow("Settings", dockSettings);
                ImGui::DockBuilderFinish(dockSettings);

                /** Setting sub-settings windows. **/
                auto docktest = ImGui::DockBuilderSplitNode(dockSettings, ImGuiDir_Down, 0.5f, nullptr, &dockSettings);
                for (int i = WindowPainterGLFW::inst().getPicturePainter()->getTexturesSize(); i >= 1; i--) {
                    std::string name = "Cam" + std::to_string(i);
                    ImGui::DockBuilderDockWindow(name.c_str(), docktest);
                }

                /** Logging window. **/
                auto dockLogging = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.3f, nullptr, &dockspace_id);
                ImGui::DockBuilderDockWindow("Logging", dockLogging);
                ImGui::DockBuilderFinish(dockLogging);

                /** Viewports windows. **/
                for (int i = 1; i <= WindowPainterGLFW::inst().getPicturePainter()->getTexturesSize(); i++) {
                    std::string name = "Viewport" + std::to_string(i);
                    ImGui::DockBuilderDockWindow(name.c_str(), dockspace_id);
                    ImGui::DockBuilderFinish(dockspace_id);
                }
                ImGui::DockBuilderFinish(dockspace_id);
            }
        }

        dockMenuBar();          //> Menu bar item
        dockSettingsWindow();   //> Settings window item
        dockLogWindow();        //> Logger Window item
        dockViewportWindows();  //> Viewport Window item

        ImGui::End();
    } else {
        ImGui::Begin("Settings");
        ImGui::Image((void*)1, ImVec2{420.0f, 320.0f});
        ImGui::Image((void*)2, ImVec2{420.0f, 320.0f});
        ImGui::Image((void*)3, ImVec2{420.0f, 320.0f});
        ImGui::Image((void*)4, ImVec2{420.0f, 320.0f});
        ImGui::End();
    }
}

void sage::GuiLayer::dockMenuBar() {
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            ImGui::MenuItem("Exit", NULL, nullptr);
            ImGui::EndMenu();
        }
        
        ImGui::BeginMenu("Edit");
        ImGui::BeginMenu("Deploy");

        if (ImGui::BeginMenu("Window")) {
            ImGui::MenuItem("Single", NULL, nullptr);
            ImGui::MenuItem("Quadra", NULL, nullptr);
            ImGui::EndMenu();
        }        

        ImGui::BeginMenu("Settings");
        ImGui::EndMenuBar();
    }
}

void sage::GuiLayer::dockSettingsWindow() {
    ImGui::Begin("Settings");
    for (int i = 1; i <= WindowPainterGLFW::inst().getPicturePainter()->getTexturesSize(); i++) {
        std::string name = "Cam" + std::to_string(i);
        ImGui::Begin(name.c_str());
        ImGui::End();
    }
    ImGui::End();
}

void sage::GuiLayer::dockLogWindow() {
    ImGui::Begin("Logging");
    ImGui::Text("Command Input 1");
    ImGui::Text("Command Input 2");
    ImGui::End();
}

void sage::GuiLayer::dockViewportWindows() {
    for (uint8_t i = 1; i <= WindowPainterGLFW::inst().getPicturePainter()->getTexturesSize(); i++) {
        std::string name = "Viewport" + std::to_string(i);
        ImGui::Begin(name.c_str());
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        float x = viewportPanelSize.x;
        float y = viewportPanelSize.y;
        ImGui::Image((void*)(intptr_t)i, ImVec2{x, y});
        ImGui::End();
    }
}

void sage::GuiLayer::setGuiWindow(GLFWwindow* win) {
    _winPtr = win;
}