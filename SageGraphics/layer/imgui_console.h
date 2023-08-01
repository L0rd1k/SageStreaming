#pragma once

#include <cstdarg>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "imgui/imgui_impl_opengl3.h"
#include "utils/log.h"

struct ImGuiLog {
    ImGuiLog()
        : scrollToBottom_(true),
          logLvls_(0) {
        txtBuffers_ = std::make_unique<std::vector<ImGuiTextBuffer>>(7, ImGuiTextBuffer());
    }
    void Clear() {
        txtBuffers_->clear();
        // txtBuffer_->clear();
    }

    void addLog(const std::string& str, const sage::LogLevels& lvl) {
        txtBuffers_->at(0).append(str.c_str());
        txtBuffers_->at((int)lvl).append(str.c_str());
    }

    void draw(const char* title, bool* p_opened = NULL) {
        ImGuiIO& io = ImGui::GetIO();
        ImGui::Begin(title, p_opened);
        ImGui::SetNextItemWidth(200);
        ImGui::Combo("##combo", &logLvls_,
                     "All\0Error\0Warning\0Trace\0Info\0Debug\0Critical\0");
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetWindowSize().x - 100);
        ImGui::Checkbox("DownAnchor", &scrollToBottom_);
        ImGui::Separator();
        if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar)) {
            ImGui::TextUnformatted(txtBuffers_->at((int)logLvls_).begin());
        }
        if (scrollToBottom_) {
            ImGui::SetScrollHereY(1.0f);
        }
        ImGui::EndChild();
        ImGui::End();
    }

private:
    std::unique_ptr<std::vector<ImGuiTextBuffer>> txtBuffers_;
    bool scrollToBottom_;
    int logLvls_;
};
