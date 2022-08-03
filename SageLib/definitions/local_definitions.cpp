#include "local_definitions.h"

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