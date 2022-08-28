#pragma once

#include <memory>

#include "local_definitions.h"
#include "utils/size.h"

namespace sage {

template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T>
using Scope = std::unique_ptr<T>;

enum class DecTypes {
    Undefined = 0,
    FFMPEG = 1,
};

enum class CVCapType {
    GSTREAMER = 0,
    FFMPEG = 1,
    V4L = 2,
    ANY = 3,
};

enum class CamTypes {
    FFMPEG = 0,
    OPENCV = 1,
    Undefined = 255
};

struct SubstanceInfo {
    SubstanceInfo() : _id(-1),
                      _fps(0),
                      camType(sage::CamTypes::Undefined),
                      decType(sage::DecTypes::Undefined),
                      format(sage::ImageFormat::Undefined) {
    }
    uint8_t _id;               //> Identificator
    uint8_t _fps;              //> Frame per second
    sage::CamTypes camType;    //> Camera Reader Type
    sage::DecTypes decType;    //> Camera Decoder Type
    sage::ImageFormat format;  //> Stream format
    sage::Size<int> _size;     //> Camera Image Size
    int64_t duration;          //> Stream duration in sec
};

}  // namespace sage

static const char* toString(sage::DecTypes decoderType) {
    switch (decoderType) {
        case sage::DecTypes::FFMPEG:
            return "FFmpeg";
        default:
            return "Undefined";
    }
}

static const char* toString(sage::CVCapType captureType) {
    switch (captureType) {
        case sage::CVCapType::GSTREAMER:
            return "Gstreamer";
        case sage::CVCapType::FFMPEG:
            return "FFmpeg";
        case sage::CVCapType::V4L:
            return "Video4Linux";
        case sage::CVCapType::ANY:
            return "Default";
        default:
            return "Undefined";
    }
}

static const char* toString(sage::CamTypes cameraType) {
    switch (cameraType) {
        case sage::CamTypes::FFMPEG:
            return "FFmpeg";
        case sage::CamTypes::OPENCV:
            return "OpenCV";
        default:
            return "Undefined";
    }
}
