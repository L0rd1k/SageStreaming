#include "imgui_layer.h"

#define IMGUI_IMPL_API
#include "../3rdParty/imgui/imgui_internal.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "window/window_painter_glfw.h"

bool sage::GuiLayer::_winManager = true;
bool sage::GuiLayer::first_time = true;
ImGuiLog sage::GuiLayer::log;

void sage::GuiLayer::init() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    // io.FontGlobalScale = 1;
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
                for (int i = WindowPainterGLFW::inst().getPicturePainter()->getTexturesCount(); i >= 1; i--) {
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
                for (int i = 0; i <= WindowPainterGLFW::inst().getPicturePainter()->getTexturesCount(); i++) {
                    std::string name = "Viewport" + std::to_string(i);
                    ImGui::DockBuilderDockWindow(name.c_str(), dockspace_id);
                    ImGui::DockBuilderFinish(dockspace_id);
                    _plotInfo.push_back(PlottingSubstInfo());
                    // _plotInfo[i]._timerFps.start();
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
    static int ffmpegcaptureType = 0;
    static int cvcaptureType = 0;
    static int decoderType = 0;
    static int textureSize = 0;
    ImGui::Combo("Reader type", &readerType,
                 "FFmpeg\0OpenCV\0");
    switch (readerType) {
        case 0:
            ImGui::Combo("Transport type", &ffmpegcaptureType,
                         "Tcp\0Udp\0V4L\0Video\0");
            break;
        case 1:
            ImGui::Combo("Capture type", &cvcaptureType,
                         "Gstreamer\0FFmpeg\0V4L\0Any\0");
            break;
    }
    ImGui::Separator();
    ImGui::Combo("Texture size", &textureSize,
                 "2048x1536\0"
                 "1024x768\0"
                 "800x600\0"
                 "640x480\0"
                 "320x240\0");

    ImGui::Combo("Decoder Type", &decoderType,
                 "FFmpeg\0");

    ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);

    ImGui::BeginGroup();
    ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));  // Leave room for 1 line below us
    ImGui::Separator();
    if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None)) {
        if (ImGui::BeginTabItem("Active cameras")) {
            const int COLUMNS_COUNT = 4;
            if (ImGui::BeginTable("table_custom_headers", COLUMNS_COUNT, ImGuiTableFlags_Borders | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable)) {
                ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableSetupColumn("Reader", ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableSetupColumn("Decoder", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("URL", ImGuiTableColumnFlags_WidthFixed);
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

                int row = 0;
                for (auto elem : substanceState) {
                    ImGui::TableNextRow();
                    for (int col = 0; col < 4; col++) {
                        ImGui::TableSetColumnIndex(col);
                        std::string str_elem = std::string();
                        if (col == 0) {
                            str_elem = std::to_string(elem.second->id);
                        } else if (col == 1) {
                            str_elem = toString(elem.second->camType);
                        } else if (col == 2) {
                            str_elem = toString(elem.second->decType);
                        } else if (col == 3) {
                            str_elem = elem.second->url;
                        }
                        if (ImGui::Selectable(str_elem.c_str(), selectedRow == row, ImGuiSelectableFlags_SpanAllColumns)) {
                            selectedRow = row;
                            Log::trace(row, col, str_elem.c_str());
                        }
                    }
                    row++;
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

    if (ImGui::Button("Create Camera")) {
        CameraState cam_state = {
            (uint8_t)-1,
            camera_url,
            static_cast<sage::CamTypes>(readerType),
            static_cast<sage::DecTypes>(decoderType),
            static_cast<sage::FFmpegType>(ffmpegcaptureType),
            static_cast<sage::OpencvType>(cvcaptureType)};
        sig_sendCameraState.emit(cam_state);
    }

    ImGui::SameLine();
    if (ImGui::Button("Remove Camera")) {
        Log::debug("Remove Camera");
    }
    ImGui::SameLine();
    if (ImGui::Button("Update Camera")) {
        Log::debug("Update Camera");
    }

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
    ImGui::Text("Cameras count: %d", WindowPainterGLFW::inst().getPicturePainter()->getTexturesCount());
    for (uint8_t i = 0; i < WindowPainterGLFW::inst().getPicturePainter()->getTexturesCount(); i++) {
        std::string name = "Cam" + std::to_string(i + 1);
        ImGui::Begin(name.c_str());
        /** Average complexity for find in unordered_map 0(1). **/
        if (substanceInfo.find(i) != substanceInfo.end()) {
            ImGui::BeginGroup();
            if (ImGui::CollapsingHeader("Status", 32)) {
                ImGui::Text("Channel ID:        %d", substanceInfo.at(i)->id);
                ImGui::Text("Fps:               %d", substanceInfo.at(i)->fps);
                ImGui::PlotLines(" ", _plotInfo.at(i)._fpsValues, 25, 0, NULL, 0.0f, 40.0f, ImVec2(300, 50));
                ImGui::Text("Resolution:        %s", substanceInfo.at(i)->size.toStr().c_str());
                ImGui::Text("Stream duration:   %ld sec.", substanceInfo.at(i)->duration);
                ImGui::Text("Reader Type:       %s", toString(substanceInfo.at(i)->camType));
                ImGui::Text("Decoder Type:      %s", toString(substanceInfo.at(i)->decType));
                ImGui::Text("Decoder Codec:     %s", toString(substanceInfo.at(i)->format));
            }
            if (ImGui::CollapsingHeader("Commands")) {
            }
            ImGui::EndGroup();
        }
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

void sage::GuiLayer::appendSubstInfo(const SubstanceState& subst) {
    std::lock_guard<std::mutex> _locker(_mtx);
    if (_plotInfo.size() > subst.id) {
        if (_plotInfo[subst.id]._timerFps.elapsed() > 1) {
            _plotInfo[subst.id]._fpsValues[_plotInfo[subst.id]._valOffset] = subst.fps;
            _plotInfo[subst.id]._valOffset =
                (_plotInfo[subst.id]._valOffset + 1) % IM_ARRAYSIZE(_plotInfo[subst.id]._fpsValues);
            _plotInfo[subst.id]._timerFps.restart();
        }
    }
    substanceInfo.insert(std::make_pair(subst.id, &subst));
}

void sage::GuiLayer::appendSubstState(const CameraState& subst) {
    std::lock_guard<std::mutex> _locker(_mtx);
    substanceState.insert(std::make_pair(subst.id, &subst));
}

void sage::GuiLayer::dockViewport() {
    std::lock_guard<std::mutex> _locker(_mtx);
    // Log::trace("C:", (int)WindowPainterGLFW::inst().getPicturePainter()->getTexturesCount());
    for (uint8_t i = 1; i <= WindowPainterGLFW::inst().getPicturePainter()->getTexturesCount(); i++) {
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