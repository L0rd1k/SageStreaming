#include "local_definitions.h"

Signal<const std::string&> sage::sig_LogMsgSend;
Signal<const sage::CameraState&> sage::sig_sendCameraState;

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

const char* toString(sage::DecTypes decoderType) {
    switch (decoderType) {
        case sage::DecTypes::FFMPEG:
            return "FFmpeg";
        default:
            return "Undefined";
    }
}

const char* toString(sage::CVCapType captureType) {
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

const char* toString(sage::RtspTransportType captureType) {
    switch (captureType) {
        case sage::RtspTransportType::Tcp:
            return "Tcp";
        case sage::RtspTransportType::Udp:
            return "Udp";
        case sage::RtspTransportType::V4l:
            return "V4l";
        case sage::RtspTransportType::Vid:
            return "Vid";
        default:
            return "Undefined";
    }
}

const char* toString(sage::CamTypes cameraType) {
    switch (cameraType) {
        case sage::CamTypes::FFMPEG:
            return "FFmpeg";
        case sage::CamTypes::OPENCV:
            return "OpenCV";
        default:
            return "Undefined";
    }
}

sage::CamTypes toCamType(std::string val) {
    if(!val.compare("ffmpeg")) {
        return sage::CamTypes::FFMPEG;
    } else if(!val.compare("opencv")) {
        return sage::CamTypes::OPENCV;
    } else {
        return sage::CamTypes::Undefined;
    }
}

sage::DecTypes toDecType(std::string val) {
    if(!val.compare("ffmpeg")) {
        return sage::DecTypes::FFMPEG;
    } else if (!val.compare("opencv")) {
    } else {
        return sage::DecTypes::Undefined;
    }
}

sage::RtspTransportType toFFmpegCapType(std::string val) {
    if(!val.compare("tcp")) {
        return sage::RtspTransportType::Tcp;
    } else if(!val.compare("udp")) {
        return sage::RtspTransportType::Udp;
    } else if(!val.compare("v4l")) {
        return sage::RtspTransportType::V4l;
    } else if(!val.compare("vid")) {
        return sage::RtspTransportType::Vid;
    } else {
        return sage::RtspTransportType::Undefined;
    }
}

sage::CVCapType toOpenCVCapType(std::string val) {
    if(!val.compare("gstreamer")) {
        return sage::CVCapType::GSTREAMER;
    } else if(!val.compare("ffmpeg")) {
        return sage::CVCapType::FFMPEG;
    } else if(!val.compare("v4l")) {
        return sage::CVCapType::V4L;
    } else if(!val.compare("any")) {
        return sage::CVCapType::ANY;
    } else {
        return sage::CVCapType::Undefined;
    }
}