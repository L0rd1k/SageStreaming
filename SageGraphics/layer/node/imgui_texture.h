#pragma once

#define IMGUI_IMPL_API
#include "imgui_node/imgui_node_editor.h"

extern "C" {
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include "stb_image.h"
}

namespace sage {

class ImGuiTexture {
public:
    static ImGuiTexture& inst() {
        static ImGuiTexture inst;
        return inst;
    }


    // ImTextureID loadTexture(const char* path) {
    //     int width = 0, height = 0, component = 0;
    //     if (auto data = stbi_load(path, &width, &height, &component, 4)) {
    //         auto texture = createTexture(data, width, height);
    //         stbi_image_free(data);
    //         return texture;
    //     } else
    //         return nullptr;
    // }
};

}  // namespace sage