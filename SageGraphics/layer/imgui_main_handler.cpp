#include "imgui_main_handler.h"

#include "window/window_painter_glfw.h"

void ImgGuiMainHandler::mainHandler2() {
}

void ImgGuiMainHandler::sideBarMenu() {
    ImGuiViewportP* viewport = (ImGuiViewportP*)(void*)ImGui::GetMainViewport();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar;
    ImGuiStyle* style = &ImGui::GetStyle();
    float textHeight = ImGui::CalcTextSize("A").y;
    ImVec2 itemSize = ImVec2{textHeight * 3.0f, textHeight * 3.0f};
    if (ImGui::BeginViewportSideBar("##MainStatusBar", viewport, ImGuiDir_Left, itemSize.x + (style->WindowPadding.x * 2), window_flags)) {
        ImGuiSelectableFlags selectableFlags = ImGuiSelectableFlags_NoPadWithHalfSpacing;
        ImGui::BringWindowToDisplayFront(ImGui::GetCurrentWindow());
        ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, style->Colors[ImGuiCol_WindowBg]);
        int i = 0;
        if (ImGui::Selectable(ICON_FA_HOME, i == selectedItem, selectableFlags, itemSize)) {
            selectedItem = i;
        }
        if (ImGui::Selectable(ICON_FA_FILE_O, i == selectedItem, selectableFlags, itemSize)) {
            selectedItem = i;
        }
        if (ImGui::Selectable(ICON_FA_VIDEO_CAMERA, i == selectedItem, selectableFlags, itemSize)) {
            selectedItem = i;
        }
        if (ImGui::Selectable(ICON_FA_SIGNAL, i == selectedItem, selectableFlags, itemSize)) {
            selectedItem = i;
        }
        if (ImGui::Selectable(ICON_FA_BOOKMARK, i == selectedItem, selectableFlags, itemSize)) {
            selectedItem = i;
        }
        if (ImGui::Selectable(ICON_FA_PICTURE_O, i == selectedItem, selectableFlags, itemSize)) {
            selectedItem = i;
        }
        ImGui::PopStyleColor();
        ImGui::PopStyleVar();  // ImGuiStyleVar_SelectableTextAlign
        ImGui::End();
    }
}

// static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
// static inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }

void ImgGuiMainHandler::mainHandler() {
    // ImGui::ShowDemoWindow(&isShowingDemo_);
    // applicationInfo(&isShowingDemo_);
    if (isDockingEnabled_) {
        createMainWindow();
        ImGuiIO& io = ImGui::GetIO();

        if (isFirstLaunch_) {
            std::cout << "First Launch" << std::endl;
            _videoNodeEditor = std::make_unique<sage::ImGuiVideoNodeEditor>();
            _videoNodeEditor->init();
            isFirstLaunch_ = false;
        }
        sideBarMenu();
        _videoNodeEditor->onFrame();

        // auto& editorStyle = ax::NodeEditor::GetStyle();
        // ImGui::BeginHorizontal("Style buttons", ImVec2(paneWidth, 0), 1.0f);
        // ImGui::TextUnformatted("Values");
        // ImGui::Spring();

        /** Check if docking is available. **/
        // if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        //     ImVec2 availableSize = ImGui::GetContentRegionAvail();
        //     dockspaceId_ = ImGui::GetID("MyDockSpace");
        //     ImGui::DockSpace(dockspaceId_, ImVec2(0.0f, 0.0f), dockspace_flags);
        //     if (isFirstLaunch_) {
        //         firstRunInit(availableSize);
        //         isFirstLaunch_ = false;
        //     }
        //     static bool isStreaming;
        //     if (isShowingWinManager_) {
        //         if (isStreaming) {
        //             sig_stopSubstances.emit();
        //             isStreaming = false;
        //         }
        //         updateManager();
        //     } else {
        //         if (!isStreaming) {
        //             sig_runSubstances.emit();
        //             isStreaming = true;
        //         }
        //         updateViewPort();
        //         updateSettingsWindow();
        //     }
        //     updateMenuBar();
        //     updateLogging();
        // }
        closeWindow();
    }
}

MapSubstState* ImgGuiMainHandler::getSubstanceState() {
    return &substanceState;
}

void ImgGuiMainHandler::firstRunInit(ImVec2& size) {
    ImVec2 availableSize = ImGui::GetContentRegionAvail();

    ImGui::DockBuilderRemoveNode(dockspaceId_);  // clear any previous layout
    ImGui::DockBuilderAddNode(dockspaceId_, ImGuiDockNodeFlags_None);
    ImGui::DockBuilderSetNodeSize(dockspaceId_, size);
    /** Settings main window. **/
    ImGuiID dockSettings = ImGui::DockBuilderSplitNode(dockspaceId_, ImGuiDir_Right, settingsWinCap, nullptr, &dockspaceId_);
    ImGui::DockBuilderDockWindow(settingsWinName.c_str(), dockSettings);
    ImGui::DockBuilderFinish(dockSettings);
    /** Setting sub-camera settings windows. **/
    ImGuiID camsSettings = ImGui::DockBuilderSplitNode(dockSettings, ImGuiDir_Down, camSettingsWinCap, nullptr, &dockSettings);

    for (int i = 0; i < WindowPainterGLFW::inst().getPicturePainter()->getTexturesCount(); i++) {
        std::string name = "Cam" + std::to_string(i + 1);
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
    if (isSplitDisplay_) {
        int rows_cols = ceil(sqrt(WindowPainterGLFW::inst().getPicturePainter()->getTexturesCount()));
        ImGuiID guiIds[rows_cols][rows_cols];
        for (int row = 0; row < rows_cols; row++) {
            for (int col = 0; col < rows_cols; col++) {
                if (row == 0 && col == 0) {
                    guiIds[row][col] = ImGui::DockBuilderSplitNode(dockspaceId_, ImGuiDir_Down, camSettingsWinCap, NULL, &dockspaceId_);
                } else if (row == 0 && col == 1) {
                    guiIds[row][col] = ImGui::DockBuilderSplitNode(dockspaceId_, ImGuiDir_Right, 0.5f, NULL, &guiIds[row][col - 1]);
                } else if (row == 0 && col != 0) {
                    guiIds[row][col] = ImGui::DockBuilderSplitNode(guiIds[row][col - 1], ImGuiDir_Right, 0.5f, NULL, &guiIds[row][col - 1]);
                } else {
                    guiIds[row][col] = ImGui::DockBuilderSplitNode(guiIds[row - 1][col], ImGuiDir_Down, 0.5f, NULL, &guiIds[row - 1][col]);
                }
            }
        }
        int counter = 1;
        for (int row = 0; row < rows_cols; row++) {
            for (int col = 0; col < rows_cols; col++) {
                std::string name = "Viewport" + std::to_string(counter);
                counter++;
                ImGui::DockBuilderDockWindow(name.c_str(), guiIds[row][col]);
            }
        }
    } else {
        ImGuiID view = ImGui::DockBuilderSplitNode(dockspaceId_, ImGuiDir_Down, camSettingsWinCap, nullptr, &dockspaceId_);
        for (int i = 1; i <= WindowPainterGLFW::inst().getPicturePainter()->getTexturesCount(); i++) {
            std::string name = "Viewport" + std::to_string(i);
            ImGui::DockBuilderDockWindow(name.c_str(), view);
            ImGui::DockBuilderFinish(dockspaceId_);
        }
    }
    ImGui::DockBuilderFinish(dockspaceId_);
}

void ImgGuiMainHandler::createMainWindow() {
    if (isFullScreenEnabled_) {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        window_flags |= ImGuiWindowFlags_NoTitleBar |            // Disable title-bar
                        ImGuiWindowFlags_NoCollapse |            // Disable user collapsing window by double-clicking on it. Also referred to as Window Menu Button (e.g. within a docking node).
                        ImGuiWindowFlags_NoResize |              // Disable user resizing with the lower-right grip
                        ImGuiWindowFlags_NoBringToFrontOnFocus;  // Disable bringing window to front when taking focus (e.g. clicking on it or programmatically giving it focus)

        // ImGuiWindowFlags_NoMove |                      // Disable user moving the window
        // ImGuiWindowFlags_NoNavFocus;               // No focusing toward this window with gamepad/keyboard navigation (e.g. skipped by CTRL+TAB)
    }
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) {
        window_flags |= ImGuiWindowFlags_NoBackground;
    }
    ImGui::Begin("CoreDisplay", &isDockSpaceOpened_, window_flags);
}

void ImgGuiMainHandler::updateSettingsWindow() {
    ImGui::Begin(settingsWinName.c_str());
    ImGui::Text("Cameras count: %d", WindowPainterGLFW::inst().getPicturePainter()->getTexturesCount());
    int counter = 0;
    for (const auto& elem : substanceState) {
        if (elem.second->isSubstEnabled) {
            updateSubstancePlot(*elem.second);
            std::string name = "Cam" + std::to_string(counter + 1);
            ImGui::Begin(name.c_str());
            ImGui::BeginGroup();
            if (ImGui::CollapsingHeader("Status", 32)) {
                ImGui::Text("Channel ID:        %d", elem.second->id);
                ImGui::Text("Fps:               %d", elem.second->fps);
                ImGui::PlotLines(" ", camSettings.at(elem.second->id).plot()->fpsArr, 25, 0, NULL, 0.0f, 40.0f, ImVec2(0, 50));
                ImGui::Text("Resolution:        %s", elem.second->size.toStr().c_str());
                ImGui::Text("Stream duration:   %ld sec.", elem.second->duration);
                ImGui::Text("Reader Type:       %s", toString(elem.second->camType).c_str());
                ImGui::Text("Decoder Type:      %s", toString(elem.second->decType).c_str());
                ImGui::Text("Decoder Codec:     %s", toString(elem.second->format).c_str());
            }
            if (ImGui::CollapsingHeader("Commands", 32)) {
                ImGui::Checkbox("Aspect ratio", camSettings.at(elem.second->id).getAspectRatio());
            }
            ImGui::EndGroup();
            ImGui::End();
            ++counter;
        }
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
        ImGui::MenuItem(managerWinName.c_str(), NULL, &isShowingWinManager_);
        if (ImGui::BeginMenu("Window")) {
            if (ImGui::MenuItem("Split Display", NULL, &isSplitDisplay_)) {
                isFirstLaunch_ = true;
            }
            ImGui::EndMenu();
        }
        ImGui::BeginMenu("Settings");
        ImGui::EndMenuBar();
    }
}

void ImgGuiMainHandler::updateViewPort() {
    std::lock_guard<std::mutex> locker(mtx_);
    int elemPos = 1;
    for (const auto& elem : substanceState) {
        if (elem.second->isSubstEnabled) {
            std::string name = "Viewport" + std::to_string(elemPos);
            ImGui::Begin(name.c_str());
            ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
            if (*camSettings[elem.first].getAspectRatio()) {
                GLint textureWidth, textureHeight;
                glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &textureWidth);
                glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &textureHeight);

                // std::cout << viewportPanelSize.x << "x" << viewportPanelSize.y << " " << textureWidth << "x" << textureHeight << std::endl;
                double ratioX = viewportPanelSize.x / (float)textureWidth;
                double ratioY = viewportPanelSize.y / (float)textureHeight;
                double ratio = (ratioX < ratioY) ? ratioX : ratioY;
                float viewWidth = textureWidth * ratio;
                float viewHeight = textureHeight * ratio;
                float viewX = (viewportPanelSize.x - textureWidth * ratio) / 2;
                float viewY = (viewportPanelSize.y - textureHeight * ratio) / 2;
                ImGui::SetCursorPos(ImVec2{viewX, viewY});
                ImGui::Image((void*)(intptr_t)elemPos, ImVec2{viewWidth, viewHeight});
            } else {
                float x = viewportPanelSize.x;
                float y = viewportPanelSize.y;
                ImGui::Image((void*)(intptr_t)elemPos, ImVec2{x, y});
            }
            ImGui::End();
            elemPos++;
        }
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
    // ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
    ImGui::BeginGroup();
    ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));  // Leave room for 1 line below us
    ImGui::Separator();
    if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None)) {
        if (ImGui::BeginTabItem("Active cameras")) {
            const int COLUMNS_COUNT = 6;
            if (ImGui::BeginTable("table_custom_headers", COLUMNS_COUNT, ImGuiTableFlags_Borders | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable)) {
                ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableSetupColumn("Reader", ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableSetupColumn("Decoder", ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableSetupColumn("URL", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("Active", ImGuiTableColumnFlags_WidthFixed);
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
                for (auto& elem : substanceState) {
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
                                        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - 30);
                                        if (ImGui::Checkbox("", camSettings[elem.second->id].getCameraActivity())) {
                                            sig_activateCamera.emit(elem.first, camSettings[elem.second->id].getCameraActivity());
                                        }
                                        break;
                                    }
                                    case 5: {
                                        if (ImGui::Button("Remove")) {
                                            sig_removeCamera.emit(elem.first);
                                            substanceState.erase(elem.first);
                                            camSettings.erase(elem.first);
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
        camState.isSubstEnabled = true;
        sig_createCamera.emit(camState);
    }
    ImGui::End();
    ImGui::Separator();
}

void ImgGuiMainHandler::updateSubstancePlot(const sage::SubstanceState& subst) {
    if (camSettings.at(subst.id).plot()->timer.elapsed() > 1) {
        camSettings.at(subst.id).plot()->fpsArr[camSettings.at(subst.id).plot()->offset] = subst.fps;
        camSettings.at(subst.id).plot()->offset =
            (camSettings.at(subst.id).plot()->offset + 1) % IM_ARRAYSIZE(camSettings.at(subst.id).plot()->fpsArr);
        camSettings.at(subst.id).plot()->timer.restart();
    }
}

void ImgGuiMainHandler::updateLogging() {
    ImGui::Begin(loggingWinName.c_str());
    ImGui::End();
}

void ImgGuiMainHandler::closeWindow() {
    ImGui::End();
}
