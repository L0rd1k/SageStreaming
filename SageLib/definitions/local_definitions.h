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

enum class CVCapType {
    GSTREAMER = 0,
    FFMPEG = 1,
    V4L = 2,
    ANY = 3,
    Undefined = 255
};

enum class RtspTransportType {
    Tcp = 0,
    Udp = 1,
    V4l = 2,
    Vid = 3,
    Undefined = 255
};

enum class CamTypes {
    FFMPEG = 0,
    OPENCV = 1,
    Undefined = 255
};

struct SubstanceInfo {
    SubstanceInfo() : id(-1),
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
};

struct CameraState {
    uint8_t id;
    std::string url;
    sage::CamTypes camType;
    sage::DecTypes decType;
    sage::RtspTransportType ffmpegcapType;
    sage::CVCapType cvcapType;
};

//< Signals
extern Signal<const std::string&> sig_LogMsgSend;
extern Signal<const CameraState&> sig_sendCameraState;

}  // namespace sage

const char* toString(sage::ImageFormat format);
const char* toString(sage::ImageSource source);
const char* toString(sage::DecTypes decoderType);
const char* toString(sage::CVCapType captureType);
const char* toString(sage::RtspTransportType cameraType);
const char* toString(sage::CamTypes cameraType);

sage::CamTypes toCamType(std::string val);
sage::DecTypes toDecType(std::string val);
sage::RtspTransportType toFFmpegCapType(std::string val);
sage::CVCapType toOpenCVCapType(std::string val);
