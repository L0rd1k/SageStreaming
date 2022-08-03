#pragma once

#include <memory>

namespace sage {

template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T>
using Scope = std::unique_ptr<T>;

enum class DecTypes {
    UNDEFINED = 0,
    FFMPEG = 1,
};

enum class CVCapType {
    GSTREAMER = 0,
    FFMPEG = 1,
    V4L = 2,
    ANY = 3,
};


}  // namespace sage