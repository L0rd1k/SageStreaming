#pragma once

#define IMGUI_IMPL_API
#include "imgui/imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "imgui_node/imgui_node_editor.h"

extern "C" {
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include "stb_image.h"
}

#include <glad/glad.h>

#include <algorithm>
namespace sage {

struct ImTexture {
    GLuint textureID = 0;
    int width = 0;
    int height = 0;
};

class ImGuiTexture {
public:
    static ImGuiTexture& inst();
    ImTextureID loadTexture(const char* path);
    ImTextureID createTexture(const void* data, int width, int height);
    int getTextureWidth(ImTextureID texture);
    int getTextureHeight(ImTextureID texture);

private:
    ImVector<sage::ImTexture>::iterator findTexture(ImTextureID texture);

private:
    ImVector<sage::ImTexture> _textures;
};

}  // namespace sage