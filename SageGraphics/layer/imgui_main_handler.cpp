#include "imgui_main_handler.h"

#include "window/window_painter_glfw.h"

void ImgGuiMainHandler::mainHandler() {
    ImGui::ShowDemoWindow(&isShowingDemo_);

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

MapSubstState* ImgGuiMainHandler::getCameraInfo() {
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

        camSettings_.insert({i, ImGuiCameraSettings()});
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
        // _plotInfo.push_back(PlottingSubstInfo());
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
                // ImGui::PlotLines(" ", _plotInfo.at(i).pltFpsValues, 25, 0, NULL, 0.0f, 40.0f, ImVec2(300, 50));

                ImGui::PlotLines(" ", camSettings_.at(i).plot()->fpsArr, 25, 0, NULL, 0.0f, 40.0f, ImVec2(300, 50));

                ImGui::Text("Resolution:        %s", cameraInfo.at(i)->size.toStr().c_str());
                ImGui::Text("Stream duration:   %ld sec.", cameraInfo.at(i)->duration);
                ImGui::Text("Reader Type:       %s", toString(cameraInfo.at(i)->camType).c_str());
                ImGui::Text("Decoder Type:      %s", toString(cameraInfo.at(i)->decType).c_str());
                ImGui::Text("Decoder Codec:     %s", toString(cameraInfo.at(i)->format).c_str());
            }
            if (ImGui::CollapsingHeader("Commands", 32)) {
                // bool* isChecked = camSettings_[i].getAspectRatio();
                // std::cout << *isChecked << std::endl;
                ImGui::Checkbox("Aspect ratio", camSettings_[i].getAspectRatio());
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
        if (*camSettings_[i - 1].getAspectRatio()) {
            GLint textureWidth, textureHeight;
            glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &textureWidth);
            glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &textureHeight);
            // std::cout << viewportPanelSize.x << "x" <<  viewportPanelSize.y << " " << textureWidth << "x" << textureHeight << std::endl;
            double ratioX = viewportPanelSize.x / (float)textureWidth;
            double ratioY = viewportPanelSize.y / (float)textureHeight;
            double ratio = (ratioX < ratioY) ? ratioX : ratioY;
            float viewWidth = textureWidth * ratio;
            float viewHeight = textureHeight * ratio;
            float viewX = (viewportPanelSize.x - textureWidth * ratio) / 2;
            float viewY = (viewportPanelSize.y - textureHeight * ratio) / 2;
            ImGui::SetCursorPos(ImVec2{viewX, viewY});
            ImGui::Image((void*)(intptr_t)i, ImVec2{viewWidth, viewHeight});
        } else {
            float x = viewportPanelSize.x;
            float y = viewportPanelSize.y;
            ImGui::Image((void*)(intptr_t)i, ImVec2{x, y});
        }
        ImGui::End();
    }
}

void ImgGuiMainHandler::updateManager() {
    ImGui::Begin(managerWinName.c_str(), &isShowingWinManager_);
    ImGui::InputText("Camera URL", txtBoxCamUrl, IM_ARRAYSIZE(txtBoxCamUrl));
    ImGui::Combo("Reader type", &readerType_, comboBoxReaderType);
    switch (readerType_) {
        case 0:
            ImGui::Combo("Transport type", &ffmpegCaptureType_, comboBoxCapTypeFFmpeg);
            break;
        case 1:
            ImGui::Combo("Capture type", &opencvCaptureType_, comboBoxCapTypeOpenCV);
            break;
    }
    ImGui::Separator();
    ImGui::Combo("Texture size", &textureSize_, comboBoxTextureSize);
    ImGui::Combo("Decoder Type", &decoderType_, comboBoxDecoderType);
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
                                        ImGui::Text("%s", toString(elem.second->camType).c_str());
                                        break;
                                    }
                                    case 2: {
                                        ImGui::Text("%s", toString(elem.second->decType).c_str());
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
        sage::SubstanceState camState;
        camState.id = (uint8_t)-1;
        camState.url = txtBoxCamUrl;
        camState.camType = static_cast<sage::CamTypes>(readerType_);
        camState.decType = static_cast<sage::DecTypes>(decoderType_);
        camState.capTypeFFmpeg = static_cast<sage::FFmpegType>(ffmpegCaptureType_);
        camState.capTypeOpencv = static_cast<sage::OpencvType>(opencvCaptureType_);
        sage::sig_sendCameraState.emit(camState);
    }
    ImGui::End();
    ImGui::Separator();
}

void ImgGuiMainHandler::updateSubstancePlot(const sage::SubstanceState& subst) {
    if (camSettings_.at(subst.id).plot()->timer.elapsed() > 1) {
        camSettings_.at(subst.id).plot()->fpsArr[camSettings_.at(subst.id).plot()->offset] = subst.fps;
        camSettings_.at(subst.id).plot()->offset =
            (camSettings_.at(subst.id).plot()->offset + 1) % IM_ARRAYSIZE(camSettings_.at(subst.id).plot()->fpsArr);
        camSettings_.at(subst.id).plot()->timer.restart();
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
