#pragma once

#include <inttypes.h>

#include <memory>

#include "callbacks/signal.h"
#include "utils/size.h"

enum class DataType {
    Image = 0,
    Undefined = 255,
};

namespace sage {

template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T>
using Scope = std::unique_ptr<T>;

const int queueSize = 3;
const uint8_t txtCount = 2;

enum class ImageFormat {
    JPEG = 0,
    H264 = 1,
    RAW = 2,
    MPEG4 = 3,
    YUYV = 4,
    BAYER = 5,
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
    BGRA = 4,
    BGRX = 5,
    RGBX = 6,
    UYVY = 7,
    YUY2 = 8,
    YVYU = 9,
    Undefined = 255
};

namespace ffmpeg {
const int timeoutConnect = 15000;
const int timeoutStream = 5000;
const int timeoutTeardown = 1000;
};  // namespace ffmpeg

enum class DecTypes {
    FFMPEG = 0,
    Undefined = 255,
};

enum class EncTypes {
    OPENCV = 0,
    Undefined = 255,
};

enum class OpencvType {
    GSTREAMER = 0,
    FFMPEG = 1,
    V4L = 2,
    ANY = 3,
    Undefined = 255
};

enum class FFmpegType {
    Tcp = 0,
    Udp = 1,
    V4l = 2,
    Vid = 3,
    Undefined = 255
};

enum class CamTypes {
    FFMPEG = 0,
    OPENCV = 1,
    GSTREAMER = 2,
    Undefined = 255
};

struct SubstanceState {
    SubstanceState() : id(-1),
                      fps(0),
                      camType(sage::CamTypes::Undefined),
                      decType(sage::DecTypes::Undefined),
                      format(sage::ImageFormat::Undefined) {
    }
    uint8_t id;                //> Identificator
    uint8_t fps;               //> Frame per second
    sage::CamTypes camType;    //> Camera Reader Type
    sage::DecTypes decType;    //> Camera Decoder Type
    sage::ImageFormat format;  //> Stream format
    sage::Size<int> size;      //> Camera Image Size
    int64_t duration;          //> Stream duration in sec

    std::string url;
    sage::FFmpegType capTypeFFmpeg;
    sage::OpencvType capTypeOpencv;

};

//< Signals
extern Signal<const std::string&> sig_LogMsgSend;
extern Signal<const SubstanceState&> sig_sendCameraState;

}  // namespace sage

std::string toString(sage::ImageFormat format);
std::string toString(sage::ImageSource source);
std::string toString(sage::DecTypes decoderType);
std::string toString(sage::OpencvType captureType);
std::string toString(sage::FFmpegType cameraType);
std::string toString(sage::CamTypes cameraType);

sage::CamTypes toCamType(std::string val);
sage::DecTypes toDecType(std::string val);
sage::EncTypes toEncType(std::string val);
sage::FFmpegType toFFmpegCapType(std::string val);
sage::OpencvType toOpenCVCapType(std::string val);
