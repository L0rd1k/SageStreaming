#pragma once

// #include "utils/gl_header.h"

// #ifdef USE_GLUT
// #include <GL/gl.h>
// #elif USE_GLFW
// #include <glad/glad.h>
// #endif

#include <GL/gl.h>

#include "definitions/local_definitions.h"
#include "utils/log.h"

static GLenum gl_GetColorType(sage::ColorType colorType) {
    switch (colorType) {
        case sage::ColorType::GRAY: {
            return GL_LUMINANCE;
        }
        case sage::ColorType::BGR: {
            return GL_BGR;
        }
        case sage::ColorType::RGBA: {
            return GL_RGBA;
        }
        case sage::ColorType::RGB: {
            return GL_RGB;
        }
    }
}

static GLenum gl_GetInternalFormat(sage::ColorType format) {
    switch (format) {
        case sage::ColorType::GRAY: {
            return GL_LUMINANCE;
        }
        case sage::ColorType::RGB: {
            return GL_RGB;
        }
        case sage::ColorType::BGR: {
            return GL_RGB;
        }
        case sage::ColorType::RGBA: {
            return GL_RGBA;
        }
        default: {
            return GL_NONE;
        }
    }
}
