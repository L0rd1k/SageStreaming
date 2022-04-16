#pragma once

enum class DataType {
    Image = 0,
    Undefined = 255,
};


namespace img {
    const int queueSize = 3;
    enum class ImageFormat {
        JPEG = 0,
        H264 = 1,
        Undefined = 255 
    };
    enum class ImageSource {
        RTSP = 0,
        DISK = 1,
        Undefined = 255
    };
};

namespace cam {
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

};
