#pragma once

#ifdef __linux__
#include <GL/gl.h>
#include <GL/glut.h>
#endif

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
