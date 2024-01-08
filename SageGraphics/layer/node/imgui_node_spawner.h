#pragma once

#define IMGUI_IMPL_API
#include "imgui_blueprint_definition.h"
#include "imgui_node/imgui_node_editor.h"

namespace sage {

class NodeSpawner {
public:
    static NodeSpawner& inst() {
        static NodeSpawner inst;
        return inst;
    }

    uint32_t getNextId() {
        return ++_nextId;
    }

    void buildNode(sage::ImGuiNode* node) {
        for (auto& input : node->getInputs()) {
            input.setNode(node);
            input.setKind(sage::PinKind::Input);
        }

        for (auto& output : node->getOuputs()) {
            output.setNode(node);
            output.setKind(sage::PinKind::Output);
        }
    }

    void buildNodes(std::vector<sage::ImGuiNode>& nodes) {
        for (auto& node : nodes) 
            buildNode(&node);
    }

    sage::ImGuiNode* spawnInputActionNode(std::vector<sage::ImGuiNode>& nodes) {
        nodes.emplace_back(getNextId(), "InputAction Fire", ImColor(255, 128, 128));
        nodes.back().getOuputs().emplace_back(getNextId(), "", sage::PinType::Delegate);
        nodes.back().getOuputs().emplace_back(getNextId(), "Pressed", sage::PinType::Flow);
        nodes.back().getOuputs().emplace_back(getNextId(), "Released", sage::PinType::Flow);
        buildNode(&nodes.back());
        return &nodes.back();
    }

    sage::ImGuiNode* spawnBranchNode(std::vector<sage::ImGuiNode>& nodes) {
        nodes.emplace_back(getNextId(), "Branch");
        nodes.back().getInputs().emplace_back(getNextId(), "", sage::PinType::Flow);
        nodes.back().getInputs().emplace_back(getNextId(), "Condition", sage::PinType::Bool);
        nodes.back().getOuputs().emplace_back(getNextId(), "True", sage::PinType::Flow);
        nodes.back().getOuputs().emplace_back(getNextId(), "False", sage::PinType::Flow);
        buildNode(&nodes.back());
        return &nodes.back();
    }

    sage::ImGuiNode* spawnDoNNode(std::vector<sage::ImGuiNode>& nodes) {
        nodes.emplace_back(getNextId(), "Do N");
        nodes.back().getInputs().emplace_back(getNextId(), "Enter", sage::PinType::Flow);
        nodes.back().getInputs().emplace_back(getNextId(), "N", sage::PinType::Int);
        nodes.back().getInputs().emplace_back(getNextId(), "Reset", sage::PinType::Flow);
        nodes.back().getOuputs().emplace_back(getNextId(), "Exit", sage::PinType::Flow);
        nodes.back().getOuputs().emplace_back(getNextId(), "Counter", sage::PinType::Int);
        buildNode(&nodes.back());
        return &nodes.back();
    }

    sage::ImGuiNode* spawnOutputActionNode(std::vector<sage::ImGuiNode>& nodes) {
        nodes.emplace_back(getNextId(), "OutputAction");
        nodes.back().getInputs().emplace_back(getNextId(), "Sample", sage::PinType::Float);
        nodes.back().getOuputs().emplace_back(getNextId(), "Condition", sage::PinType::Bool);
        nodes.back().getInputs().emplace_back(getNextId(), "Event", sage::PinType::Delegate);
        buildNode(&nodes.back());
        return &nodes.back();
    }

    sage::ImGuiNode* spawnPrintStringNode(std::vector<sage::ImGuiNode>& nodes) {
        nodes.emplace_back(getNextId(), "Print String");
        nodes.back().getInputs().emplace_back(getNextId(), "", sage::PinType::Flow);
        nodes.back().getInputs().emplace_back(getNextId(), "In String", sage::PinType::String);
        nodes.back().getOuputs().emplace_back(getNextId(), "", sage::PinType::Flow);
        buildNode(&nodes.back());
        return &nodes.back();
    }

    sage::ImGuiNode* spawnMessageNode(std::vector<sage::ImGuiNode>& nodes) {
        nodes.emplace_back(getNextId(), "", ImColor(128, 195, 248));
        nodes.back().setNodeType(sage::NodeType::Simple);
        nodes.back().getOuputs().emplace_back(getNextId(), "Message", sage::PinType::String);
        buildNode(&nodes.back());
        return &nodes.back();
    }

    sage::ImGuiNode* spawnSetTimerNode(std::vector<sage::ImGuiNode>& nodes) {
        nodes.emplace_back(getNextId(), "Set Timer", ImColor(128, 195, 248));
        nodes.back().getInputs().emplace_back(getNextId(), "", sage::PinType::Flow);
        nodes.back().getInputs().emplace_back(getNextId(), "Object", sage::PinType::Object);
        nodes.back().getInputs().emplace_back(getNextId(), "Function Name", sage::PinType::Func);
        nodes.back().getInputs().emplace_back(getNextId(), "Time", sage::PinType::Float);
        nodes.back().getInputs().emplace_back(getNextId(), "Looping", sage::PinType::Bool);
        nodes.back().getOuputs().emplace_back(getNextId(), "", sage::PinType::Flow);
        buildNode(&nodes.back());
        return &nodes.back();
    }

    sage::ImGuiNode* spawnLessNode(std::vector<sage::ImGuiNode>& nodes) {
        nodes.emplace_back(getNextId(), "<", ImColor(128, 195, 248));
        nodes.back().setNodeType(sage::NodeType::Simple);
        nodes.back().getInputs().emplace_back(getNextId(), "", sage::PinType::Float);
        nodes.back().getInputs().emplace_back(getNextId(), "", sage::PinType::Float);
        nodes.back().getOuputs().emplace_back(getNextId(), "", sage::PinType::Float);
        buildNode(&nodes.back());
        return &nodes.back();
    }

    sage::ImGuiNode* spawnWeirdNode(std::vector<sage::ImGuiNode>& nodes) {
        nodes.emplace_back(getNextId(), "o.O", ImColor(128, 195, 248));
        nodes.back().setNodeType(sage::NodeType::Simple);
        nodes.back().getInputs().emplace_back(getNextId(), "", sage::PinType::Float);
        nodes.back().getOuputs().emplace_back(getNextId(), "", sage::PinType::Float);
        nodes.back().getOuputs().emplace_back(getNextId(), "", sage::PinType::Float);
        buildNode(&nodes.back());
        return &nodes.back();
    }

    sage::ImGuiNode* SpawnTraceByChannelNode(std::vector<sage::ImGuiNode>& nodes) {
        nodes.emplace_back(getNextId(), "Single Line Trace by Channel", ImColor(255, 128, 64));
        nodes.back().getInputs().emplace_back(getNextId(), "", sage::PinType::Flow);
        nodes.back().getInputs().emplace_back(getNextId(), "Start", sage::PinType::Flow);
        nodes.back().getInputs().emplace_back(getNextId(), "End", sage::PinType::Int);
        nodes.back().getInputs().emplace_back(getNextId(), "Trace Channel", sage::PinType::Float);
        nodes.back().getInputs().emplace_back(getNextId(), "Trace Complex", sage::PinType::Bool);
        nodes.back().getInputs().emplace_back(getNextId(), "Actors to Ignore", sage::PinType::Int);
        nodes.back().getInputs().emplace_back(getNextId(), "Draw Debug Type", sage::PinType::Bool);
        nodes.back().getInputs().emplace_back(getNextId(), "Ignore Self", sage::PinType::Bool);
        nodes.back().getOuputs().emplace_back(getNextId(), "", sage::PinType::Flow);
        nodes.back().getOuputs().emplace_back(getNextId(), "Out Hit", sage::PinType::Float);
        nodes.back().getOuputs().emplace_back(getNextId(), "Return Value", sage::PinType::Bool);
        buildNode(&nodes.back());
        return &nodes.back();
    }

    sage::ImGuiNode* spawnTreeSequenceNode(std::vector<sage::ImGuiNode>& nodes) {
        nodes.emplace_back(getNextId(), "Sequence");
        nodes.back().setNodeType(sage::NodeType::Tree);
        nodes.back().getInputs().emplace_back(getNextId(), "", sage::PinType::Flow);
        nodes.back().getOuputs().emplace_back(getNextId(), "", sage::PinType::Flow);
        buildNode(&nodes.back());
        return &nodes.back();
    }

    sage::ImGuiNode* spawnTreeTaskNode(std::vector<sage::ImGuiNode>& nodes) {
        nodes.emplace_back(getNextId(), "Move To");
        nodes.back().setNodeType(sage::NodeType::Tree);
        nodes.back().getInputs().emplace_back(getNextId(), "", sage::PinType::Flow);
        buildNode(&nodes.back());
        return &nodes.back();
    }

    sage::ImGuiNode* spawnTreeTask2Node(std::vector<sage::ImGuiNode>& nodes) {
        nodes.emplace_back(getNextId(), "Random Wait");
        nodes.back().setNodeType(sage::NodeType::Tree);
        nodes.back().getInputs().emplace_back(getNextId(), "", sage::PinType::Flow);
        buildNode(&nodes.back());
        return &nodes.back();
    }

    sage::ImGuiNode* spawnComment(std::vector<sage::ImGuiNode>& nodes) {
        nodes.emplace_back(getNextId(), "Test Comment");
        nodes.back().setNodeType(sage::NodeType::Comment);
        nodes.back().setSize(ImVec2(300, 200));
        buildNode(&nodes.back());  // TODO
        return &nodes.back();
    }

    sage::ImGuiNode* spawnHoudiniTransformNode(std::vector<sage::ImGuiNode>& nodes) {
        nodes.emplace_back(getNextId(), "Transform");
        nodes.back().setNodeType(sage::NodeType::Houdini);
        nodes.back().getInputs().emplace_back(getNextId(), "", sage::PinType::Flow);
        nodes.back().getOuputs().emplace_back(getNextId(), "", sage::PinType::Flow);
        buildNode(&nodes.back());
        return &nodes.back();
    }

    sage::ImGuiNode* spawnHoudiniGroupNode(std::vector<sage::ImGuiNode>& nodes) {
        nodes.emplace_back(getNextId(), "Group");
        nodes.back().setNodeType(sage::NodeType::Houdini);
        nodes.back().getInputs().emplace_back(getNextId(), "", sage::PinType::Flow);
        nodes.back().getInputs().emplace_back(getNextId(), "", sage::PinType::Flow);
        nodes.back().getOuputs().emplace_back(getNextId(), "", sage::PinType::Flow);
        buildNode(&nodes.back());
        return &nodes.back();
    }

private:
    uint32_t _nextId = 1;
};

}  // namespace sage