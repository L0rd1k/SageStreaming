#include "local_definitions.h"

Signal<const std::string&> sage::sig_LogMsgSend;

const char* toString(sage::ImageFormat format) {
    switch (format) {
    case sage::ImageFormat::H264:
        return "H264";
    case sage::ImageFormat::JPEG:
        return "JPEG";
    case sage::ImageFormat::RAW:
        return "RAW";
    case sage::ImageFormat::MPEG4:
        return "MPEG4";
    case sage::ImageFormat::YUYV:
        return "YUYV";
    default:
        return "Undefined";
    }        
}

const char* toString(sage::ImageSource source) {
    switch (source) {
    case sage::ImageSource::RTSP:
        return "RTSP";
    case sage::ImageSource::DISK:
        return "DISK";
    default:
        return "Undefined";
    }        
}