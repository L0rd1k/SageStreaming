#include "local_definitions.h"

Signal<const std::string&> sage::sig_LogMsgSend;
Signal<const sage::SubstanceState&> sage::sig_sendCameraState;

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

const char* toString(sage::OpencvType captureType) {
    switch (captureType) {
        case sage::OpencvType::GSTREAMER:
            return "Gstreamer";
        case sage::OpencvType::FFMPEG:
            return "FFmpeg";
        case sage::OpencvType::V4L:
            return "Video4Linux";
        case sage::OpencvType::ANY:
            return "Default";
        default:
            return "Undefined";
    }
}

const char* toString(sage::FFmpegType captureType) {
    switch (captureType) {
        case sage::FFmpegType::Tcp:
            return "Tcp";
        case sage::FFmpegType::Udp:
            return "Udp";
        case sage::FFmpegType::V4l:
            return "V4l";
        case sage::FFmpegType::Vid:
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
        case sage::CamTypes::GSTREAMER:
            return "Gstreamer";
        default:
            return "Undefined";
    }
}

sage::CamTypes toCamType(std::string val) {
    if(!val.compare("ffmpeg")) {
        return sage::CamTypes::FFMPEG;
    } else if(!val.compare("opencv")) {
        return sage::CamTypes::OPENCV;
    } else if(!val.compare("gstreamer")) {
        return sage::CamTypes::GSTREAMER;
    } else {
        return sage::CamTypes::Undefined;
    }
}

sage::DecTypes toDecType(std::string val) {
    if(!val.compare("ffmpeg")) {
        return sage::DecTypes::FFMPEG;
    } else if (!val.compare("opencv")) {
        return sage::DecTypes::Undefined;
    } else {
        return sage::DecTypes::Undefined;
    }
}

sage::EncTypes toEncType(std::string val) {
    if(!val.compare("opencv")) {
        return sage::EncTypes::OPENCV;
    } else {
        return sage::EncTypes::Undefined;
    }
}

sage::FFmpegType toFFmpegCapType(std::string val) {
    if(!val.compare("tcp")) {
        return sage::FFmpegType::Tcp;
    } else if(!val.compare("udp")) {
        return sage::FFmpegType::Udp;
    } else if(!val.compare("v4l")) {
        return sage::FFmpegType::V4l;
    } else if(!val.compare("vid")) {
        return sage::FFmpegType::Vid;
    } else {
        return sage::FFmpegType::Undefined;
    }
}

sage::OpencvType toOpenCVCapType(std::string val) {
    if(!val.compare("gstreamer")) {
        return sage::OpencvType::GSTREAMER;
    } else if(!val.compare("ffmpeg")) {
        return sage::OpencvType::FFMPEG;
    } else if(!val.compare("v4l")) {
        return sage::OpencvType::V4L;
    } else if(!val.compare("any")) {
        return sage::OpencvType::ANY;
    } else {
        return sage::OpencvType::Undefined;
    }
}