#include "imgui_video_node_editor.h"

// ImVec2 sage::ImGuiVideoNodeEditor::_scroll = ImVec2(0.0f, 0.0f);
bool sage::ImGuiVideoNodeEditor::_isInited = false;
// bool sage::ImGuiVideoNodeEditor::_isShowGrid = true;

sage::ImGuiVideoNodeEditor::ImGuiVideoNodeEditor()
    : _editor(nullptr) {
}

void sage::ImGuiVideoNodeEditor::init() {
    if (!_isInited) {
        configurate();
        _isInited = true;
    }
}

void sage::ImGuiVideoNodeEditor::configurate() {
    std::cout << "Configurate" << std::endl;
    ax::NodeEditor::Config config;
    config.SettingsFile = "Blueprints.json";
    config.UserPointer = this;
    config.LoadNodeSettings = [](ax::NodeEditor::NodeId id,
                                 char* data,
                                 void* uptr) -> size_t {
        auto self = static_cast<ImGuiVideoNodeEditor*>(uptr);
        sage::ImGuiNode* node = self->findNode(id);
        if (!node) {
            return 0;
        }
        if (data != nullptr) {
            memcpy(data, node->getState().data(), node->getState().size());
        }
        return node->getState().size();
    };
    config.SaveNodeSettings = [](ax::NodeEditor::NodeId id,
                                 const char* data,
                                 size_t size,
                                 ax::NodeEditor::SaveReasonFlags reason,
                                 void* uptr) -> bool {
        auto self = static_cast<ImGuiVideoNodeEditor*>(uptr);
        sage::ImGuiNode* node = self->findNode(id);
        if (node) {
            node->getState().assign(data, size);
            self->touchNode(id);
            return true;
        }
        return false;
    };

    if (!_editor) {
        std::cout << "Create Editor" << std::endl;
        _editor = ax::NodeEditor::CreateEditor(&config);
        ax::NodeEditor::SetCurrentEditor(_editor);
    }
    spawnNodeElements();
    ax::NodeEditor::NavigateToContent();
    NodeSpawner().inst().buildNodes(_nodes);
}

void sage::ImGuiVideoNodeEditor::spawnNodeElements() {
    sage::ImGuiNode* node;
    node = NodeSpawner().inst().spawnInputActionNode(_nodes);
    node = NodeSpawner().inst().spawnInputActionNode(_nodes);
    ax::NodeEditor::SetNodePosition(node->getId(), ImVec2(-252, 220));
    node = NodeSpawner().inst().spawnBranchNode(_nodes);
    ax::NodeEditor::SetNodePosition(node->getId(), ImVec2(-300, 351));
    node = NodeSpawner().inst().spawnDoNNode(_nodes);
    ax::NodeEditor::SetNodePosition(node->getId(), ImVec2(-238, 504));
    node = NodeSpawner().inst().spawnOutputActionNode(_nodes);
    ax::NodeEditor::SetNodePosition(node->getId(), ImVec2(71, 80));
    node = NodeSpawner().inst().spawnSetTimerNode(_nodes);
    ax::NodeEditor::SetNodePosition(node->getId(), ImVec2(168, 316));

    node = NodeSpawner().inst().spawnTreeSequenceNode(_nodes);
    ax::NodeEditor::SetNodePosition(node->getId(), ImVec2(1028, 329));
    node = NodeSpawner().inst().spawnTreeTaskNode(_nodes);
    ax::NodeEditor::SetNodePosition(node->getId(), ImVec2(1204, 458));
    node = NodeSpawner().inst().spawnTreeTask2Node(_nodes);
    ax::NodeEditor::SetNodePosition(node->getId(), ImVec2(868, 538));

    node = NodeSpawner().inst().spawnComment(_nodes);
    ax::NodeEditor::SetNodePosition(node->getId(), ImVec2(112, 576));
    node = NodeSpawner().inst().spawnComment(_nodes);
    ax::NodeEditor::SetNodePosition(node->getId(), ImVec2(800, 224));

    node = NodeSpawner().inst().spawnLessNode(_nodes);
    ax::NodeEditor::SetNodePosition(node->getId(), ImVec2(366, 652));
    node = NodeSpawner().inst().spawnWeirdNode(_nodes);
    ax::NodeEditor::SetNodePosition(node->getId(), ImVec2(144, 652));
    node = NodeSpawner().inst().spawnMessageNode(_nodes);
    ax::NodeEditor::SetNodePosition(node->getId(), ImVec2(-348, 698));
    node = NodeSpawner().inst().spawnPrintStringNode(_nodes);
    ax::NodeEditor::SetNodePosition(node->getId(), ImVec2(-69, 652));

    node = NodeSpawner().inst().spawnHoudiniTransformNode(_nodes);
    ax::NodeEditor::SetNodePosition(node->getId(), ImVec2(500, -70));
    node = NodeSpawner().inst().spawnHoudiniGroupNode(_nodes);
    ax::NodeEditor::SetNodePosition(node->getId(), ImVec2(500, 42));
}

void sage::ImGuiVideoNodeEditor::onFrame() {
    ImGui::Begin("Style");
    static float left_pane_width = 300.0f;
    static float right_pane_width = 800.0f;
    splitter(true, 4.0f, &left_pane_width, &right_pane_width, 50.0f, 50.0f);
    showLeftPanel(left_pane_width - 4.0f);
    ImGui::SameLine(0.0f, 12.0f);

    ax::NodeEditor::Begin("Node editor");
    auto cursorTopLeft = ImGui::GetCursorScreenPos();
    for (auto& node : _nodes) {
        if (node.getNodeType() != sage::NodeType::Blueprint &&
            node.getNodeType() != sage::NodeType::Simple) {
            continue;
        }
        const auto isSimple = node.getNodeType() == sage::NodeType::Simple;
        bool hasOutputDelegates = false;
        for (auto& output : node.getOuputs()) {
            if (output.getType() == sage::PinType::Delegate) {
                hasOutputDelegates = true;
            }
        }

    }

    ax::NodeEditor::End();

    ImGui::End();

    // auto& editorStyle = ax::NodeEditor::GetStyle();
    // ImGui::BeginHorizontal("Style buttons", ImVec2(paneWidth, 0), 1.0f);
    // ImGui::TextUnformatted("Values");
    // ImGui::Spring();
}

void sage::ImGuiVideoNodeEditor::showLeftPanel(float width) {
    auto& io = ImGui::GetIO();
    ImGui::BeginChild("Selection", ImVec2(width, 0));
    width = ImGui::GetContentRegionAvail().x;
    if (ImGui::Button("Show Flow")) {
        for (auto& link : _links)
            ax::NodeEditor::Flow(link.getId());
    }

    std::vector<ax::NodeEditor::NodeId> selectedNodes;
    std::vector<ax::NodeEditor::LinkId> selectedLinks;
    // std::cout << ax::NodeEditor::GetSelectedObjectCount() << std::endl;
    selectedNodes.resize(ax::NodeEditor::GetSelectedObjectCount());
    selectedLinks.resize(ax::NodeEditor::GetSelectedObjectCount());

    int nodeCount = ax::NodeEditor::GetSelectedNodes(selectedNodes.data(), static_cast<int>(selectedNodes.size()));
    int linkCount = ax::NodeEditor::GetSelectedLinks(selectedLinks.data(), static_cast<int>(selectedLinks.size()));

    selectedNodes.resize(nodeCount);
    selectedLinks.resize(linkCount);

    ImGui::GetWindowDrawList()->AddRectFilled(
        ImGui::GetCursorScreenPos(),
        ImGui::GetCursorScreenPos() + ImVec2(width, ImGui::GetTextLineHeight()),
        ImColor(ImGui::GetStyle().Colors[ImGuiCol_HeaderActive]), ImGui::GetTextLineHeight() * 0.25f);
    ImGui::Spacing();
    ImGui::SameLine();
    ImGui::TextUnformatted("Nodes");

    ImGui::Indent();
    for (auto& node : _nodes) {
        ImGui::PushID(node.getId().AsPointer());
        auto start = ImGui::GetCursorScreenPos();
        if (const auto progress = getTouchProgress(node.getId())) {
            ImGui::GetWindowDrawList()->AddLine(
                start + ImVec2(-8, 0),
                start + ImVec2(-8, ImGui::GetTextLineHeight()),
                IM_COL32(255, 0, 0, 255 - (int)(255 * progress)), 4.0f);
        }
        bool isSelected = std::find(selectedNodes.begin(), selectedNodes.end(), node.getId()) != selectedNodes.end();
        if (ImGui::Selectable((node.getName() + "##" + std::to_string(reinterpret_cast<uintptr_t>(node.getId().AsPointer()))).c_str(), &isSelected)) {
            if (io.KeyCtrl) {
                if (isSelected) {
                    ax::NodeEditor::SelectNode(node.getId(), true);
                } else {
                    ax::NodeEditor::DeselectNode(node.getId());
                }
            } else {
                ax::NodeEditor::SelectNode(node.getId(), false);
            }
            ax::NodeEditor::NavigateToSelection();
        }
        if (ImGui::IsItemHovered() && !node.getState().empty()) {
            ImGui::SetTooltip("State: %s", node.getState().c_str());
        }
        auto id = std::string("(") + std::to_string(reinterpret_cast<uintptr_t>(node.getId().AsPointer())) + ")";
        auto textSize = ImGui::CalcTextSize(id.c_str(), nullptr);
        // auto iconPanelPos = start + ImVec2(
        //                                 width - ImGui::GetStyle().FramePadding.x - ImGui::GetStyle().IndentSpacing - saveIconWidth - restoreIconWidth - ImGui::GetStyle().ItemInnerSpacing.x * 1,
        //                                 (ImGui::GetTextLineHeight() - saveIconHeight) / 2);
        // ImGui::GetWindowDrawList()->AddText(
        //     ImVec2(textSize.x - ImGui::GetStyle().ItemInnerSpacing.x, start.y),
        //     IM_COL32(255, 255, 255, 255), id.c_str(), nullptr);
        auto drawList = ImGui::GetWindowDrawList();
        ImGui::PopID();
    }
    ImGui::Unindent();
    static int changeCount = 0;

    ImGui::GetWindowDrawList()->AddRectFilled(
        ImGui::GetCursorScreenPos(),
        ImGui::GetCursorScreenPos() + ImVec2(width, ImGui::GetTextLineHeight()),
        ImColor(ImGui::GetStyle().Colors[ImGuiCol_HeaderActive]), ImGui::GetTextLineHeight() * 0.25f);
    ImGui::Spacing();
    ImGui::SameLine();
    ImGui::TextUnformatted("Selection");

    // ImGui::BeginHorizontal("Selection Stats", ImVec2(width, 0));
    ImGui::Text("Changed %d time%s", changeCount, changeCount > 1 ? "s" : "");
    ImGui::SameLine();
    if (ImGui::Button("Deselect All"))
        ax::NodeEditor::ClearSelection();
    // ImGui::EndHorizontal();
    ImGui::Indent();
    for (int i = 0; i < nodeCount; ++i) ImGui::Text("Node (%p)", selectedNodes[i].AsPointer());
    for (int i = 0; i < linkCount; ++i) ImGui::Text("Link (%p)", selectedLinks[i].AsPointer());
    ImGui::Unindent();

    ImGui::EndChild();
}

bool sage::ImGuiVideoNodeEditor::splitter(bool split_v, float thickness, float* left_size,
                                          float* right_size, float min_left_size, float min_right_size,
                                          float splitter_size) {
    ImGuiContext& imguiCtx = *GImGui;
    ImGuiWindow* window = imguiCtx.CurrentWindow;
    ImGuiID id = window->GetID("##Splitter");
    ImRect bbox;
    bbox.Min = window->DC.CursorPos + (split_v ? ImVec2(*left_size, 0.0f) : ImVec2(0.0f, *left_size));
    bbox.Max = bbox.Min + ImGui::CalcItemSize(split_v ? ImVec2(thickness, splitter_size)
                                                      : ImVec2(splitter_size, thickness),
                                              0.0f, 0.0f);
    return ImGui::SplitterBehavior(bbox, id, split_v ? ImGuiAxis_X : ImGuiAxis_Y,
                                   left_size, right_size,
                                   min_left_size, min_right_size, 0.0f);
}

/*************************************************************/
/***********************  NODE HANDLING **********************/
/*************************************************************/

sage::ImGuiNode* sage::ImGuiVideoNodeEditor::findNode(ax::NodeEditor::NodeId id) {
    for (auto& node : _nodes) {
        if (node.getId() == id) {
            return &node;
        }
    }
    return nullptr;
}

void sage::ImGuiVideoNodeEditor::touchNode(ax::NodeEditor::NodeId id) {
    _nodeTouchDT[id] = _touchTime;
}

float sage::ImGuiVideoNodeEditor::getTouchProgress(ax::NodeEditor::NodeId id) {
    auto it = _nodeTouchDT.find(id);
    if (it != _nodeTouchDT.end() && it->second > 0.0f)
        return (_touchTime - it->second) / _touchTime;
    else
        return 0.0f;
}