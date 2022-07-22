#pragma once

#ifdef __linux__
#include <GL/gl.h>
#include <GL/glut.h>
#endif

#include "definitions/local_definitions.h"
#include "utils/Log.h"

static GLenum gl_GetColorType(img::ColorType colorType) {
    switch (colorType) {
        case img::ColorType::GRAY: {
            return GL_LUMINANCE;
        }
        case img::ColorType::BGR: {
            return GL_BGR;
        }
        case img::ColorType::RGBA: {
            return GL_RGBA;
        }
        case img::ColorType::RGB: {
            return GL_RGB;
        }
    }
}

static GLenum gl_GetInternalFormat(img::ColorType format) {
    switch (format) {
        case img::ColorType::GRAY: {
            return GL_LUMINANCE;
        }
        case img::ColorType::RGB: {
            return GL_RGB;
        }
        case img::ColorType::BGR: {
            return GL_RGB;
        }
        case img::ColorType::RGBA: {
            return GL_RGBA;
        }
        default: {
            return GL_NONE;
        }
    }
}
