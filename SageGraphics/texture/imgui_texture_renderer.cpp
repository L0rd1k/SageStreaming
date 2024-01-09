#include "imgui_texture_renderer.h"

sage::ImGuiTexture& sage::ImGuiTexture::inst() {
    static sage::ImGuiTexture inst;
    return inst;
}

ImTextureID sage::ImGuiTexture::loadTexture(const char* path) {
    int width = 0, height = 0, component = 0;
    if (auto data = stbi_load(path, &width, &height, &component, 4)) {
        auto texture = createTexture(data, width, height);
        stbi_image_free(data);
        return texture;
    } else
        return nullptr;
}

ImTextureID sage::ImGuiTexture::createTexture(const void* data, int width, int height) {
    _textures.resize(_textures.size() + 1);
    ImTexture& texture = _textures.back();
    GLint lastTexture = 0;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTexture);
    glGenTextures(1, &texture.textureID);
    glBindTexture(GL_TEXTURE_2D, texture.textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, lastTexture);
    texture.width = width;
    texture.height = height;
    return reinterpret_cast<ImTextureID>(static_cast<std::intptr_t>(texture.textureID));
}

int sage::ImGuiTexture::getTextureWidth(ImTextureID texture) {
    auto textureIt = findTexture(texture);
    if (textureIt != _textures.end()) {
        return textureIt->width;
    }
    return 0;
}

int sage::ImGuiTexture::getTextureHeight(ImTextureID texture) {
    auto textureIt = findTexture(texture);
    if (textureIt != _textures.end()) {
        return textureIt->height;
    }
    return 0;
}

ImVector<sage::ImTexture>::iterator sage::ImGuiTexture::findTexture(ImTextureID texture) {
    auto textureID = static_cast<GLuint>(reinterpret_cast<std::intptr_t>(texture));
    return std::find_if(_textures.begin(), _textures.end(), [textureID](sage::ImTexture& texture) {
        return texture.textureID == textureID;
    });
}
