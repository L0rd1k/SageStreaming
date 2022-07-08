#include "FFmpegUtils.h"

bool initializeFFmpeg(int logLevel) {
    static bool inited = false;
    static std::mutex mtx;
    std::lock_guard<std::mutex> lock(mtx);
    if(!inited) {
        av_register_all();
        avdevice_register_all();
        avcodec_register_all();
        avformat_network_init();
        av_log_set_level(logLevel);
        inited = true;
        return true;
    }
    return false;
}

std::string errorToString(int code) {
    const int size = 2048;
    char buffer[size];
    av_strerror(code, buffer, size);
    return std::string(buffer);      
}

RtspCameraState errortoCameraState(int code, bool timerExpired) {
    std::map<int, RtspCameraState> errors;
    errors[AVERROR_STREAM_NOT_FOUND] = RtspCameraState::NoConnection;
    errors[AVERROR_HTTP_BAD_REQUEST] = RtspCameraState::HttpBadRequest;
    errors[AVERROR_HTTP_UNAUTHORIZED] = RtspCameraState::HttpUnauthorized;
    errors[AVERROR_HTTP_FORBIDDEN] = RtspCameraState::HttpForbidden;
    errors[AVERROR_HTTP_NOT_FOUND] = RtspCameraState::HttpNotFound;
    errors[AVERROR_HTTP_OTHER_4XX] = RtspCameraState::HttpOther;
    errors[AVERROR_HTTP_SERVER_ERROR] = RtspCameraState::HttpServerError;
    errors[AVERROR_INVALIDDATA] = RtspCameraState::InvalidData;
    errors[AVERROR_EOF] = RtspCameraState::StreamEnded;
    errors[-113] = RtspCameraState::NoConnection;
    RtspCameraState err = (errors.find(code) != errors.end()) ?
         errors[code] : RtspCameraState::Undefined;
    if(timerExpired) {
        err = RtspCameraState::BlockingTimer;
    }
    return err;
}