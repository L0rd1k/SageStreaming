#include "Texture.h"

Texture::Texture(GLuint id) :
_id(id) {
}

Texture::~Texture() {
}

void Texture::setId(GLuint id) {
    Log() << "Set id: " << id;
    _id = id;
}