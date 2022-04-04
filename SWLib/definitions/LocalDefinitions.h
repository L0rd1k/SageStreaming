#pragma once

#include "image/Image.h"

enum class DataType {
    Image = 0,
    Undefined = 255
};

enum class ImageFomat {
   JPEG = 0,
   H264 = 1,
   Undefined = 255 
};

namespace cam {

namespace ffmpeg {
    const int timeoutConnect = 15000;
    const int timeoutStream = 5000;
    const int timeoutTeardown = 1000;
};

};
