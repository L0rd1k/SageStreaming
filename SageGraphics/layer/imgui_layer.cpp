#include "imgui_layer.h"

#define IMGUI_IMPL_API
#include "../3rdParty/imgui/imgui_internal.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
// #include "imgui_console.h"
#include "window/window_painter_glfw.h"

// static ImGuiLog log;
bool sage::GuiLayer::_winManager = true;
ImGuiLog sage::GuiLayer::log;

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
        static bool _winManage = false;
        static bool first_time = true;
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
            dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

            if (_winManager) {
                winManager();
            }

            if (first_time) {
                first_time = false;
                ImGui::DockBuilderRemoveNode(dockspace_id);  // clear any previous layout
                ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_DockSpace);
                ImGui::DockBuilderSetNodeSize(dockspace_id, availableSize);

                /** Settings window. **/
                ImGuiID dockSettings = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.3f, nullptr, &dockspace_id);
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

                ImGuiID dockManager = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.7f, nullptr, &dockspace_id);
                ImGui::DockBuilderDockWindow("Manager", dockManager);
                ImGui::DockBuilderFinish(dockManager);

                /** Viewports windows. **/
                for (int i = WindowPainterGLFW::inst().getPicturePainter()->getTexturesSize(); i > 0; i--) {
                    std::string name = "Viewport" + std::to_string(i);
                    ImGui::DockBuilderDockWindow(name.c_str(), dockspace_id);
                    ImGui::DockBuilderFinish(dockspace_id);
                }
                ImGui::DockBuilderFinish(dockspace_id);
            }
        }

        dockMenuBar();   //> Menu bar item
        dockSettings();  //> Settings window item
        dockLog();       //> Logger Window item
        dockViewport();  //> Viewport Window item

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

void sage::GuiLayer::winManager() {
    // static Target opt_target = Target_Tab;
    ImGui::Begin("Manager", &_winManager);

    static char camera_url[256] = "";
    ImGui::InputText("Camera URL", camera_url, IM_ARRAYSIZE(camera_url));
    static int readerType = 0;
    static int decoderType = 0;
    static int cvcaptureType = 0;
    static int textureSize = 0;
    ImGui::SetNextItemWidth(ImGui::GetFontSize() * 9);
    ImGui::Combo("Reader type", &readerType, "FFmpeg\0OpenCV\0");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetFontSize() * 9);
    switch (readerType) {
        case 0:
            ImGui::Combo("Transport type", &decoderType, "Video\0Tcp\0Udp\0V4L\0");
            break;
        case 1:
            ImGui::Combo("Capture type", &cvcaptureType, "Gstreamer\0FFmpeg\0V4L\0Any\0");
            break;
    }

    ImGui::Combo("Texture size", &textureSize,
                 "\'2048x1536'\0\'1024x768'\0\'800x600'\0\'640x480'\0\'320x240'\0");
    ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);

    ImGui::BeginGroup();
    ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));  // Leave room for 1 line below us
    ImGui::Separator();
    if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None)) {
        if (ImGui::BeginTabItem("Active cameras")) {
            if (ImGui::TreeNode("Selection State: Single Selection")) {
                static int selected = -1;
                for (int n = 0; n < 5; n++) {
                    char buf[32];
                    sprintf(buf, "Object %d", n);
                    if (ImGui::Selectable(buf, selected == n))
                        selected = n;
                }
                ImGui::TreePop();
            }

            const int COLUMNS_COUNT = 3;
            if (ImGui::BeginTable("table_custom_headers", COLUMNS_COUNT, ImGuiTableFlags_Borders | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable)) {
                ImGui::TableSetupColumn("Camera URL");
                ImGui::TableSetupColumn("ID");
                ImGui::TableSetupColumn("Reader");

                // static bool column_selected[3] = {};
                static int selectedRow = -1;

                // Instead of calling TableHeadersRow() we'll submit custom headers ourselves
                ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
                for (int column = 0; column < COLUMNS_COUNT; column++) {
                    ImGui::TableSetColumnIndex(column);
                    const char* column_name = ImGui::TableGetColumnName(column);  // Retrieve name passed to TableSetupColumn()
                    ImGui::PushID(column);
                    ImGui::TableHeader(column_name);
                    ImGui::PopID();
                }

                for (int row = 0; row < 30; row++) {
                    ImGui::TableNextRow();
                    for (int column = 0; column < 3; column++) {
                        char buf[32];
                        sprintf(buf, "Camera %d,%d", column, row);
                        ImGui::TableSetColumnIndex(column);
                        if (ImGui::Selectable(buf, selectedRow == row, ImGuiSelectableFlags_SpanAllColumns)) {
                            selectedRow = row;
                            Log::trace(row);
                        }
                    }
                }
                ImGui::EndTable();
            }
            ImGui::SameLine();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Details")) {
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::EndChild();
    ImGui::EndGroup();

    bool isCameraCreated = ImGui::Button("Create Camera");
    ImGui::SameLine();
    bool isCameraRemoved = ImGui::Button("Remove Camera");
    ImGui::SameLine();
    bool isCameraUpdated = ImGui::Button("Update Camera");

    ImGui::End();
    ImGui::Separator();
}

void sage::GuiLayer::dockMenuBar() {
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            ImGui::MenuItem("Exit", NULL, nullptr);
            ImGui::EndMenu();
        }
        ImGui::BeginMenu("Edit");
        ImGui::BeginMenu("Deploy");
        ImGui::MenuItem("Manager", NULL, &_winManager);
        if (ImGui::BeginMenu("Window")) {
            ImGui::MenuItem("Single", NULL, nullptr);
            ImGui::MenuItem("Quadra", NULL, nullptr);
            ImGui::EndMenu();
        }
        ImGui::BeginMenu("Settings");
        ImGui::EndMenuBar();
    }
}

void sage::GuiLayer::dockSettings() {
    ImGui::Begin("Settings");
    ImGui::Text("Cameras count: %d", WindowPainterGLFW::inst().getPicturePainter()->getTexturesSize());

    for (int i = 1; i <= WindowPainterGLFW::inst().getPicturePainter()->getTexturesSize(); i++) {
        std::string name = "Cam" + std::to_string(i);
        ImGui::Begin(name.c_str());
        ImGui::Text("Channel ID:        %d", substanceInfo.at(i - 1)->_id);
        ImGui::Text("Fps:               %d", substanceInfo.at(i - 1)->_fps);
        ImGui::Text("Resolution:        %s", substanceInfo.at(i - 1)->_size.toStr().c_str());
        ImGui::Text("Stream duration:   %ld sec.", substanceInfo.at(i - 1)->duration);
        ImGui::Text("Reader Type:       %s", toString(substanceInfo.at(i - 1)->camType));
        ImGui::Text("Decoder Type:      %s", toString(substanceInfo.at(i - 1)->decType));
        ImGui::Text("Decoder Codec:     %s", toString(substanceInfo.at(i - 1)->format));
        ImGui::End();
    }
    ImGui::End();
}

void sage::GuiLayer::dockLog() {
    ImGui::Begin("Logging");
    ImGui::End();
    log.Draw("Logging");
}

void sage::GuiLayer::appendLog(const std::string& str) {
    log.AddLog(str.c_str());
}

void sage::GuiLayer::appendSubstInfo(const SubstanceInfo& subst) {
    std::lock_guard<std::mutex> _locker(_mtx);
    substanceInfo.insert(std::make_pair(subst._id, &subst));
}

void sage::GuiLayer::dockViewport() {
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