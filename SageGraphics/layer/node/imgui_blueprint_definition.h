#pragma once

#define IMGUI_IMPL_API
#include <string>
#include <vector>

#include "imgui_internal.h"
#include "imgui_node/imgui_node_editor.h"

namespace sage {

enum class PinType {
    Flow,
    Bool,
    Int,
    Float,
    String,
    Object,
    Func,
    Delegate,
};

enum class PinKind {
    Output,
    Input,
};

enum class NodeType {
    Blueprint,
    Simple,
    Tree,
    Comment,
    Houdini,
};

class ImGuiNode;

class ImGuiPin {
public:
    ImGuiPin(int32_t id, const char* name, sage::PinType type)
        : _id(id),
          _node(nullptr),
          _name(name),
          _type(type),
          _kind(sage::PinKind::Input) {
    }

    const ax::NodeEditor::PinId& getId() { return _id; }
    const sage::ImGuiNode* getNode() { return _node; }
    const std::string& getName() { return _name; }
    const sage::PinType& getType() { return _type; }
    const sage::PinKind& getKind() { return _kind; }

    void setId(int32_t id) { _id = id; }
    void setNode(sage::ImGuiNode* node) { _node = node; }
    void setName(std::string& name) { _name = name; }
    void setType(sage::PinType& type) { _type = type; }
    void setKind(sage::PinKind kind) { _kind = kind; }

private:
    ax::NodeEditor::PinId _id;
    sage::ImGuiNode* _node;
    std::string _name;
    sage::PinType _type;
    sage::PinKind _kind;
};

class ImGuiNode {
public:
    ImGuiNode(int32_t id, const char* name,
              ImColor color = ImColor(255, 255, 255))
        : _id(id),
          _name(name),
          _color(color),
          _type(sage::NodeType::Blueprint),
          _size(0, 0) {
    }

    const ax::NodeEditor::NodeId& getId() { return _id; }
    const std::string& getName() { return _name; }
    std::string& getState() { return _state; }
    std::string& getSavedState() { return _saved_state; }
    const ImColor& getColor() { return _color; }
    const sage::NodeType& getNodeType() { return _type; };
    const ImVec2& getSize() { return _size; }
    std::vector<sage::ImGuiPin>& getInputs() { return _inputs; }
    std::vector<sage::ImGuiPin>& getOuputs() { return _outputs; }

    void setId(int32_t id) { _id = id; }
    void setName(std::string& name) { _name = name; }
    void setState(std::string& state) { _state = state; }
    void setSavedState(std::string& state) { _saved_state = state; }
    void setColor(ImColor& color) { _color = color; }
    void setNodeType(sage::NodeType type) { _type = type; }
    void setSize(ImVec2 size) { _size = size; }

private:
    ax::NodeEditor::NodeId _id;
    std::string _name;
    std::string _state;
    std::string _saved_state;
    std::vector<sage::ImGuiPin> _inputs;
    std::vector<sage::ImGuiPin> _outputs;
    ImColor _color;
    sage::NodeType _type;
    ImVec2 _size;
};

class ImGuiLink {
public:
    ImGuiLink(ax::NodeEditor::LinkId id,
              ax::NodeEditor::PinId startPinId,
              ax::NodeEditor::PinId endPinId)
        : _id(id),
          _startPinId(startPinId),
          _endPinId(endPinId),
          _color(255, 255, 255) {
    }

    const ax::NodeEditor::LinkId& getId() { return _id; }
    const ax::NodeEditor::PinId& getStartPinId() { return _startPinId; }
    const ax::NodeEditor::PinId& getEndPinId() { return _endPinId; }
    const ImColor& getColor() { return _color; }

    void setId(ax::NodeEditor::LinkId id) { _id = id; }
    void setStartPinId(ax::NodeEditor::PinId& startPinId) { _startPinId = startPinId; }
    void setEndPinId(ax::NodeEditor::PinId& endPinId) { _endPinId = endPinId; }
    void setColor(ImColor& color) { _color = color; }

private:
    ax::NodeEditor::LinkId _id;
    ax::NodeEditor::PinId _startPinId;
    ax::NodeEditor::PinId _endPinId;
    ImColor _color;
};

struct ComparatorNodeIdLess {
    bool operator()(const ax::NodeEditor::NodeId& lhs,
                    const ax::NodeEditor::NodeId& rhs) const {
        return lhs.AsPointer() < rhs.AsPointer();
    }
};

}  // namespace sage