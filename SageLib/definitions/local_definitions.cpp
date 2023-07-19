#include "local_definitions.h"

Signal<const std::string&> sage::sig_LogMsgSend;
Signal<const sage::SubstanceState&> sage::sig_sendCameraState;

std::string toString(sage::ImageFormat format) {
    switch (format) {
        case sage::ImageFormat::H264:
            return "h264";
        case sage::ImageFormat::JPEG:
            return "jpeg";
        case sage::ImageFormat::RAW:
            return "raw";
        case sage::ImageFormat::MPEG4:
            return "mpeg4";
        case sage::ImageFormat::YUYV:
            return "yuyv";
        default:
            return "undefined";
    }
}

std::string toString(sage::ImageSource source) {
    switch (source) {
        case sage::ImageSource::RTSP:
            return "rtsp";
        case sage::ImageSource::DISK:
            return "disk";
        default:
            return "undefined";
    }
}

std::string toString(sage::DecTypes decoderType) {
    switch (decoderType) {
        case sage::DecTypes::FFMPEG:
            return "ffmpeg";
        default:
            return "undefined";
    }
}

std::string toString(sage::OpencvType captureType) {
    switch (captureType) {
        case sage::OpencvType::GSTREAMER:
            return "gstreamer";
        case sage::OpencvType::FFMPEG:
            return "ffmpeg";
        case sage::OpencvType::V4L:
            return "v4l";
        case sage::OpencvType::ANY:
            return "any";
        default:
            return "undefined";
    }
}

std::string toString(sage::FFmpegType captureType) {
    switch (captureType) {
        case sage::FFmpegType::Tcp:
            return "tcp";
        case sage::FFmpegType::Udp:
            return "udp";
        case sage::FFmpegType::V4l:
            return "v4l";
        case sage::FFmpegType::Vid:
            return "vid";
        default:
            return "undefined";
    }
}

std::string toString(sage::CamTypes cameraType) {
    switch (cameraType) {
        case sage::CamTypes::FFMPEG:
            return "ffmpeg";
        case sage::CamTypes::OPENCV:
            return "opencv";
        case sage::CamTypes::GSTREAMER:
            return "gstreamer";
        default:
            return "undefined";
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