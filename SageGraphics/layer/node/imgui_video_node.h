#pragma once

#define IMGUI_IMPL_API
#include "imgui_internal.h"

namespace sage {

class Node {
public:
    Node(int id,
         const char* name,
         const ImVec2& pos,
         float value,
         const ImVec4& color,
         int inputs_cnt,
         int outputs_cnt)
        : _id(id),
          _pos(pos),
          _value(value),
          _color(color),
          _inputs_cnt(inputs_cnt),
          _outputs_cnt(outputs_cnt) {
        strcpy(_name, name);
    }

    const int getID() { return _id; }
    const int getInputsCnt() { return _inputs_cnt; }
    const int getOutputsCnt() { return _outputs_cnt; }
    const float getValue() { return _value; }
    const char* getName() { return _name; }
    const ImVec2 getPos() { return _pos; }
    const ImVec2 getSize() { return _size; }
    const ImVec4 getColor() { return _color; }

    void setID(int id) { _id = id; }
    void setInputsCnt(int inputs_cnt) { _inputs_cnt = inputs_cnt; }
    void setOutputsCnt(int outputs_cnt) { _outputs_cnt = outputs_cnt; }
    void setValue(float value) { _value = value; }
    void setName(const char* name) { strcpy(_name, name); }
    void setPos(const ImVec2& pos) { _pos = pos; }
    void setSize(const ImVec2& size) { _size = size; }
    void setColor(ImVec4& color) { _color = color; }

    ImVec2 GetInputSlotPos(int slot_no) const {
        return ImVec2(_pos.x, _pos.y + _size.y * ((float)slot_no + 1) / ((float)_inputs_cnt + 1));
    }

    ImVec2 GetOutputSlotPos(int slot_no) const {
        return ImVec2(_pos.x + _size.x, _pos.y + _size.y * ((float)slot_no + 1) / ((float)_outputs_cnt + 1));
    }

private:
    int _id;
    int _inputs_cnt;
    int _outputs_cnt;
    float _value;
    char _name[64];
    ImVec2 _pos;
    ImVec2 _size;
    ImVec4 _color;
};

class NodeLink {
public:
    NodeLink(int input_idx, int input_slot, int output_idx, int output_slot)
        : _input_idx(input_idx),
          _input_slot(input_slot),
          _output_idx(output_idx),
          _output_slot(output_slot) {
    }
private:
    int _input_idx, _input_slot, _output_idx, _output_slot;
};

class ImGuiVideoNode {
};

}  // namespace sage