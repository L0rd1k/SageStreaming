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
            updateManager();
            updateMenuBar();
            updateLogging();
            updateViewPort();
            updateSettingsWindow();
        }
        closeWindow();
    }
}

MapSubstState* ImgGuiMainHandler::getSubstanceState() {
    return &substanceState;
}

MapCameraInfo* ImgGuiMainHandler::getCameraInfo() {
    return &cameraInfo;
}

void ImgGuiMainHandler::firstRunInit(ImVec2& size) {
    ImVec2 availableSize = ImGui::GetContentRegionAvail();
    ImGui::DockBuilderRemoveNode(dockspaceId_);  // clear any previous layout
    ImGui::DockBuilderAddNode(dockspaceId_, ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_DockSpace);
    ImGui::DockBuilderSetNodeSize(dockspaceId_, size);

    /** Settings main window. **/
    ImGuiID dockSettings = ImGui::DockBuilderSplitNode(dockspaceId_, ImGuiDir_Right, settingsWinCap, nullptr, &dockspaceId_);
    ImGui::DockBuilderDockWindow(settingsWinName.c_str(), dockSettings);
    ImGui::DockBuilderFinish(dockSettings);
    /** Setting sub-camera settings windows. **/
    ImGuiID camsSettings = ImGui::DockBuilderSplitNode(dockSettings, ImGuiDir_Down, camSettingsWinCap, nullptr, &dockSettings);
    for (int i = WindowPainterGLFW::inst().getPicturePainter()->getTexturesCount(); i >= 1; --i) {
        std::string name = "Cam" + std::to_string(i);
        ImGui::DockBuilderDockWindow(name.c_str(), camsSettings);
    }
    /** Logging windows **/
    ImGuiID dockLogging = ImGui::DockBuilderSplitNode(dockspaceId_, ImGuiDir_Down, loggingWinCap, nullptr, &dockspaceId_);
    ImGui::DockBuilderDockWindow(loggingWinName.c_str(), dockLogging);
    ImGui::DockBuilderFinish(dockLogging);

    /** Logging windows **/
    ImGuiID dockManager = ImGui::DockBuilderSplitNode(dockspaceId_, ImGuiDir_Right, managerWinCap, nullptr, &dockspaceId_);
    ImGui::DockBuilderDockWindow(managerWinName.c_str(), dockManager);
    ImGui::DockBuilderFinish(dockManager);

    /** Create Viewport Windows **/
    for (int i = 0; i <= WindowPainterGLFW::inst().getPicturePainter()->getTexturesCount(); i++) {
        std::string name = "Viewport" + std::to_string(i);
        ImGui::DockBuilderDockWindow(name.c_str(), dockspaceId_);
        ImGui::DockBuilderFinish(dockspaceId_);
        _plotInfo.push_back(PlottingSubstInfo());
    }

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

void ImgGuiMainHandler::updateSettingsWindow() {
    ImGui::Begin(settingsWinName.c_str());
    ImGui::Text("Cameras count: %d", WindowPainterGLFW::inst().getPicturePainter()->getTexturesCount());
    for (uint8_t i = 0; i < WindowPainterGLFW::inst().getPicturePainter()->getTexturesCount(); i++) {
        std::string name = "Cam" + std::to_string(i + 1);
        ImGui::Begin(name.c_str());
        /** Average complexity for find in unordered_map 0(1). **/
        if (cameraInfo.find(i) != cameraInfo.end()) {
            ImGui::BeginGroup();
            if (ImGui::CollapsingHeader("Status", 32)) {
                ImGui::Text("Channel ID:        %d", cameraInfo.at(i)->id);
                ImGui::Text("Fps:               %d", cameraInfo.at(i)->fps);
                
                ImGui::PlotLines(" ", _plotInfo.at(i).pltFpsValues, 25, 0, NULL, 0.0f, 40.0f, ImVec2(300, 50));
                
                ImGui::Text("Resolution:        %s", cameraInfo.at(i)->size.toStr().c_str());
                ImGui::Text("Stream duration:   %ld sec.", cameraInfo.at(i)->duration);
                ImGui::Text("Reader Type:       %s", toString(cameraInfo.at(i)->camType));
                ImGui::Text("Decoder Type:      %s", toString(cameraInfo.at(i)->decType));
                ImGui::Text("Decoder Codec:     %s", toString(cameraInfo.at(i)->format));
            }
            if (ImGui::CollapsingHeader("Commands")) {
            }
            ImGui::EndGroup();
        }
        ImGui::End();
    }
    ImGui::End();
}

void ImgGuiMainHandler::updateMenuBar() {
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            ImGui::MenuItem("Exit", NULL, nullptr);
            ImGui::EndMenu();
        }
        ImGui::BeginMenu("Edit");
        ImGui::BeginMenu("Deploy");
        ImGui::MenuItem("Manager", NULL, &isShowingWinManager_);
        if (ImGui::BeginMenu("Window")) {
            ImGui::MenuItem("Single", NULL, nullptr);
            ImGui::MenuItem("Quadra", NULL, nullptr);
            ImGui::EndMenu();
        }
        ImGui::BeginMenu("Settings");
        ImGui::EndMenuBar();
    }
}

void ImgGuiMainHandler::updateViewPort() {
    std::lock_guard<std::mutex> locker(mtx_);
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

void ImgGuiMainHandler::updateManager() {
    ImGui::Begin(managerWinName.c_str(), &isShowingWinManager_);
    char camera_url[256];
    ImGui::InputText("Camera URL", camera_url, IM_ARRAYSIZE(camera_url));
    ImGui::Combo("Reader type", &readerType_, combobox_readerTypes);
    switch (readerType_) {
        case 0:
            ImGui::Combo("Transport type", &ffmpegCaptureType_, combobox_ffmpegCapTypes);
            break;
        case 1:
            ImGui::Combo("Capture type", &opencvCaptureType_, combobox_opencvCapTypes);
            break;
    }
    ImGui::Separator();
    ImGui::Combo("Texture size", &textureSize_, combobox_textureSize);
    ImGui::Combo("Decoder Type", &decoderType_, combobox_decoderType);
    ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);

    ImGui::BeginGroup();
    ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));  // Leave room for 1 line below us
    ImGui::Separator();
    if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None)) {
        if (ImGui::BeginTabItem("Active cameras")) {
            const int COLUMNS_COUNT = 5;
            if (ImGui::BeginTable("table_custom_headers", COLUMNS_COUNT, ImGuiTableFlags_Borders | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable)) {
                ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableSetupColumn("Reader", ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableSetupColumn("Decoder", ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableSetupColumn("URL", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("Remove", ImGuiTableColumnFlags_WidthFixed);
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
                for (const auto& elem : substanceState) {
                    ImGui::PushID(std::to_string(elem.second->id).c_str());
                    ImGui::TableNextRow(ImGuiTableRowFlags_None, 0.0f);
                    for (auto cols = 0; cols < COLUMNS_COUNT; ++cols) {
                        if (ImGui::TableSetColumnIndex(cols)) {
                            if (elem.second) {
                                switch (cols) {
                                    case 0: {
                                        ImGui::AlignTextToFramePadding();
                                        if (ImGui::Selectable(std::to_string(elem.second->id).c_str(), selectedRow == row, ImGuiSelectableFlags_AllowItemOverlap, ImVec2(0, 0))) {
                                            selectedRow = row;
                                        }
                                        break;
                                    }
                                    case 1: {
                                        ImGui::Text("%s", toString(elem.second->camType));
                                        break;
                                    }
                                    case 2: {
                                        ImGui::Text("%s", toString(elem.second->decType));
                                        break;
                                    }
                                    case 3: {
                                        ImGui::Text("%s", elem.second->url.c_str());
                                        break;
                                    }
                                    case 4: {
                                        if (ImGui::Button("Remove")) {
                                            std::cout << "Remove element: " << row << "-" << cols << std::endl;
                                        }
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    row++;
                    ImGui::PopID();
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
        // CameraState cam_state = {
        //     (uint8_t)-1,
        //     camera_url,
        //     static_cast<sage::CamTypes>(readerType),
        //     static_cast<sage::DecTypes>(decoderType),
        //     static_cast<sage::FFmpegType>(ffmpegcaptureType),
        //     static_cast<sage::OpencvType>(cvcaptureType)};
        // sig_sendCameraState.emit(cam_state);
    }
    ImGui::End();
    ImGui::Separator();
}

void ImgGuiMainHandler::updateSubstancePlot(const sage::SubstanceState& subst) {
    if (_plotInfo.size() > subst.id) {
        if (_plotInfo[subst.id].pltTimerFps.elapsed() > 1) {
            _plotInfo[subst.id].pltFpsValues[_plotInfo[subst.id].pltValOffset] = subst.fps;
            _plotInfo[subst.id].pltValOffset =
                (_plotInfo[subst.id].pltValOffset + 1) % IM_ARRAYSIZE(_plotInfo[subst.id].pltFpsValues);
            _plotInfo[subst.id].pltTimerFps.restart();
        }
    }
}

void ImgGuiMainHandler::updateLogging() {
    ImGui::Begin(loggingWinName.c_str());
    ImGui::End();
    // log.Draw("Logging");
}

void ImgGuiMainHandler::closeWindow() {
    ImGui::End();
}
