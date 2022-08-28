#pragma once

#include <iostream>
#include <string>
#include <cstdarg>

#include "imgui/imgui_impl_opengl3.h"

struct ImGuiLog {
    ImGuiTextBuffer Buf;
    bool ScrollToBottom;

    void Clear() { Buf.clear(); }

    void AddLog(const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        Buf.appendfv(fmt, args);
        va_end(args);
        ScrollToBottom = true;
    }

    void Draw(const char* title, bool* p_opened = NULL) {
        ImGui::Begin(title, p_opened);
        ImGui::TextUnformatted(Buf.begin());
        if (ScrollToBottom)
            ImGui::SetScrollHereY(1.0f);
        ScrollToBottom = false;
        ImGui::End();
    }
};
