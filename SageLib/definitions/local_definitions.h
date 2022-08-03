#pragma once

#include <inttypes.h>

enum class DataType {
    Image = 0,
    Undefined = 255,
};


namespace sage {
    const int queueSize = 3;
    enum class ImageFormat {
        JPEG = 0,
        H264 = 1,
        RAW = 2,
        MPEG4 = 3,
        Undefined = 255 
    };
    enum class ImageSource {
        RTSP = 0,
        DISK = 1,
        Undefined = 255
    };

    enum class ColorType {
        GRAY = 0,
        RGB = 1,
        BGR = 2,
        RGBA = 3,
        Undefined = 255
    };

    namespace ffmpeg {
        const int timeoutConnect = 15000;
        const int timeoutStream = 5000;
        const int timeoutTeardown = 1000;
    };

    enum class CamTypes {
        FFMPEG = 0,
        OPENCV = 1,
        Undefined = 255
    };

    // GL definitions
    const uint8_t txtCount = 2;
}


const char* toString(sage::ImageFormat format);
const char* toString(sage::ImageSource source);