#include "imgui_video_node_editor.h"

ImVec2 sage::ImGuiVideoNodeEditor::_scroll = ImVec2(0.0f, 0.0f);
bool sage::ImGuiVideoNodeEditor::_isInited = false;
bool sage::ImGuiVideoNodeEditor::_isShowGrid = true;

void sage::ImGuiVideoNodeEditor::init() {
    if (_isInited) {
        _nodes.push_back(Node(0, "MainTex", ImVec2(40, 50), 0.5f, ImColor(255, 100, 100), 1, 1));
        _nodes.push_back(Node(1, "BumpMap", ImVec2(40, 150), 0.42f, ImColor(200, 100, 200), 1, 1));
        _nodes.push_back(Node(2, "Combine", ImVec2(270, 80), 1.0f, ImColor(0, 200, 100), 2, 2));
        _links.push_back(NodeLink(0, 0, 2, 0));
        _links.push_back(NodeLink(1, 0, 2, 1));
        _isInited = true;
    }
}

void sage::ImGuiVideoNodeEditor::onFrame() {
    static bool show = true;
    if (!ImGui::Begin("Style", &show)) {
    }
    ImGui::End();

    auto& editorStyle = ax::NodeEditor::GetStyle();
    ImGui::BeginHorizontal("Style buttons", ImVec2(paneWidth, 0), 1.0f);
    ImGui::TextUnformatted("Values");
    ImGui::Spring();
}
