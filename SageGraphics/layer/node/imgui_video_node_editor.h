#pragma once

#define IMGUI_IMPL_API
#include <algorithm>
#include <iostream>
#include <map>

#include "imgui_blueprint_definition.h"
#include "imgui_internal.h"
#include "imgui_node/imgui_node_editor.h"
#include "imgui_node_spawner.h"
#include "layer/utils/imgui_utils.h"
#include "imgui_texture.h"

namespace sage {

class ImGuiVideoNodeEditor {
public:
    ImGuiVideoNodeEditor();
    void init();
    void onFrame();

private:
    bool splitter(bool split_v, float thickness, float* left_size,
                  float* right_size, float min_left_size, float min_right_size,
                  float splitter_size = -1.0);
    void showLeftPanel(float width);
    void configurate();
    void spawnNodeElements();

private:
    sage::ImGuiNode* findNode(ax::NodeEditor::NodeId id);
    void touchNode(ax::NodeEditor::NodeId id);
    float getTouchProgress(ax::NodeEditor::NodeId id);

private:
    std::vector<sage::ImGuiNode> _nodes;
    std::vector<sage::ImGuiLink> _links;

    const float _touchTime = 1.0f;
    std::map<ax::NodeEditor::NodeId, float, ComparatorNodeIdLess> _nodeTouchDT;
    static bool _isInited;
    ax::NodeEditor::EditorContext* _editor;

    ImTextureID _headerBackground = nullptr;
    ImTextureID _saveIcon = nullptr;
    ImTextureID _restoreIcon = nullptr;
};

}  // namespace sage
