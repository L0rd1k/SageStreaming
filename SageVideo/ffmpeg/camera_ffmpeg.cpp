#include "camera_ffmpeg.h"

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

CameraFFmpeg::CameraFFmpeg(std::string url, sage::FFmpegType type)
    : sage::CamerasHandler(),
      _url(url),
      _rtspTransportType(type),
      _blockTimerExp(false),
      _previousPacket(0) {
    initializeFFmpeg();
}

std::string CameraFFmpeg::getName() {
    return "FFmpeg";
}

bool CameraFFmpeg::start() {
    if (isStreaming()) {
        Log::critical("Can't start camera, already playing");
        return false;
    }
    _isStreaming.store(true);
    _camThread = std::thread(&CameraFFmpeg::mainLoop, this);
    return true;
}

bool CameraFFmpeg::stop() {
    if (_camThread.joinable()) {
        _isStreaming.store(false);
        _camThread.join();
        if(_context) {
            /** @warning Custom method from prebuild FFMPEG 2.8 **/
            closeContext();
        }
    }
}

void CameraFFmpeg::mainLoop() {
    if (prepareContext() && openContext()) {
        while (isStreaming() && receiveContext())
            _blockTimer.restart();
    }
    if (!isStreaming())
        _rtspState = RtspCameraState::Stopped;
    closeContext();
}

bool CameraFFmpeg::receiveContext() {
    AVPacket* packet = av_packet_alloc();  //> make
    int code = av_read_frame(_context, packet);
    if (code != 0) {
        handleError(code);
        return false;
    }
    if (_videoStream && packet->stream_index == _videoStream->index) {
        handleVideoFrame(_videoStream, packet);
    }
    av_packet_unref(packet);
    av_packet_free(&packet);

    return true;
}

void CameraFFmpeg::performFpsDelay(AVStream* stream, AVPacket* packet) {
    int delay = 0;
    if (packet->pts == AV_NOPTS_VALUE) {
        // Log() << stream->time_base.num  << " / " << stream->time_base.den;
        delay = 1000000 * stream->time_base.num / stream->time_base.den;
    } else if (_previousPacket > 0) {
        // Log() << "(" << packet->pts << "-" << _previousPacket << ")" <<  " * " << 1000000 << " * " << stream->time_base.num << " / " << stream->time_base.den;
        delay = (packet->pts - _previousPacket) * 1000000 * stream->time_base.num / stream->time_base.den;
    }
    if (delay > 0) {
        usleep(delay);
    }
    _previousPacket = packet->pts;
}

bool CameraFFmpeg::handleVideoFrame(AVStream* stream, AVPacket* packet) {
    std::lock_guard<std::mutex> locker(_mutex);

    imgFormat = sage::ImageFormat::Undefined;
    if (stream->codecpar->codec_id == AV_CODEC_ID_MJPEG) {
        // Log::info("[FFmpeg][Reader] MJPEG");
        imgFormat = sage::ImageFormat::JPEG;
    } else if (stream->codecpar->codec_id == AV_CODEC_ID_H264) {
        // Log::info("[FFmpeg][Reader] H264");
        imgFormat = sage::ImageFormat::H264;
    } else if (stream->codecpar->codec_id == AV_CODEC_ID_RAWVIDEO) {
        // Log::info("[FFmpeg][Reader] RAWVIDEO");
        imgFormat = sage::ImageFormat::RAW;
    } else if (stream->codecpar->codec_id == AV_CODEC_ID_MPEG4) {
        // Log::info("[FFmpeg][Reader] MPEG4");
        imgFormat = sage::ImageFormat::MPEG4;
    } else {
        Log::error("[FFmpeg][Reader] Unsupported image format:", std::to_string(stream->codecpar->codec_id));
        return false;
    }
    // Log::info("[FFmpeg][Reader] Packet size:", packet->size);
    if (!packet->data || !packet->size) {
        return false;
    }
    if (_rtspTransportType == sage::FFmpegType::Vid) {
        performFpsDelay(stream, packet);
    }

    sage::swImage& image = _queue.next();
    if (imgFormat == sage::ImageFormat::JPEG) {
        TrimmedAVPacket trimmed = trimAVPacket(packet);
        image.setBytes(trimmed.data, trimmed.size);
    } else {
        image.setBytes(packet->data, packet->size);
    }

    if (_context->duration > 0) {
        image->duration = _context->duration;
    } else {
        image->duration = 0;
    }
    image->imgFormat = imgFormat;
    image->imgSize = sage::Size<int>(stream->codecpar->width, stream->codecpar->height);

    triggerImage(image);
    return true;
}

TrimmedAVPacket CameraFFmpeg::trimAVPacket(AVPacket* packet) {
    int startOffset = 0;
    int endOffset = 0;
    for (int i = 0; i < packet->size - 1; i++) {
        uint8_t curr = packet->data[i];
        uint8_t next = packet->data[i + 1];
        if (curr == 0xFF && next == 0xD8) {
            startOffset = i;
        }
        if (curr == 0xFF && next == 0xD9) {
            endOffset = i;
            break;
        }
    }

    TrimmedAVPacket trimmed;
    trimmed.data = packet->data + startOffset;
    trimmed.size = endOffset - startOffset + 2;
    return trimmed;
}

int blockingOperationCallback(void* sender) {
    CameraFFmpeg* ffmpegClient = (CameraFFmpeg*)sender;
    return ffmpegClient->blockingTimerExpired();
}

bool CameraFFmpeg::blockingTimerExpired() {
    _blockTimerExp = _blockTimer.expired(_blockTimerTimeout);
    return !_isStreaming.load() || _blockTimerExp;
}

bool CameraFFmpeg::prepareContext() {
    Log::info("[FFmpeg][Reader] Preparare context.");
    _isStreaming.store(true);

    _rtspState = RtspCameraState::Undefined;

    _context = avformat_alloc_context();
    _context->interrupt_callback.callback = blockingOperationCallback;
    _context->interrupt_callback.opaque = this;

    _blockTimer.restart();
    _blockTimerTimeout = sage::ffmpeg::timeoutConnect;
    _blockTimerExp = false;

    AVDictionary* dict = nullptr;
    const AVInputFormat* inputFormat = NULL;
    if (_rtspTransportType == sage::FFmpegType::Tcp) {
        av_dict_set(&dict, "rtps_transport", "tcp", 0);
    } else if (_rtspTransportType == sage::FFmpegType::Udp) {
        av_dict_set(&dict, "rtps_transport", "udp", 0);
    } else if (_rtspTransportType == sage::FFmpegType::V4l) {
        inputFormat = av_find_input_format("v4l2");
        // Log::trace(inputFormat->long_name);
        av_dict_set(&dict, "framerate", "25", 0);
    } else if (_rtspTransportType == sage::FFmpegType::Vid) {
        av_dict_set(&dict, "framerate", "25", 0);
    } else {
        Log::error("[FFmpeg][Reader] Unknown transport type");
        return false;
    }
    int resCode = avformat_open_input(&_context, _url.c_str(), NULL, &dict);

    if (resCode != 0 || _blockTimerExp) {
        Log::error("[FFmpeg][Reader] Video input error found:", _url.c_str());
        handleError(resCode);
        return false;
    }
    return true;
}

void CameraFFmpeg::logStreamInfo() {
    Log::info("\n",
              " > Format name:", _context->iformat->name, "\n",
              " > Bitrate:", _context->bit_rate, "\n",
              " > Duratrion:", _context->duration, "\n",
              " > Codec id:", _context->streams[0]->codecpar->codec_id, "\n",
              " > Channels:", _context->streams[0]->codecpar->channels);
}

bool CameraFFmpeg::openContext() {
    int code = avformat_find_stream_info(_context, NULL); /** Get input stream information. **/
    logStreamInfo();                                      /** Log context stream information. **/
    if (code != 0 || _blockTimerExp) {
        Log::error("[FFmpeg][Reader] OpenContext Error");
        handleError(code);
        return false;
    }

    for (uint i = 0; i < _context->nb_streams; i++) {
        if (_context->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            // Log::trace("[FFMPEG] AVMEDIA_TYPE_VIDEO:", i);
            _videoStream = _context->streams[i];
        } else if (_context->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            // Log() << "[FFMPEG] AVMEDIA_TYPE_AUDIO";
            _audioStream = _context->streams[i];
        } else if (_context->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_DATA) {
            // Log() << "[FFMPEG] AVMEDIA_TYPE_DATA";
        }
    }
    _blockTimerTimeout = sage::ffmpeg::timeoutStream;
    return true;
}

bool CameraFFmpeg::closeContext() {
    _blockTimerTimeout = sage::ffmpeg::timeoutTeardown;
    _blockTimer.restart();
    avformat_close_input(&_context);
    avformat_free_context(_context);
    _context = NULL;
    _previousPacket = 0;
}

void CameraFFmpeg::handleError(int resCode) {
    std::string error;
    if (_blockTimerExp) {
        error = "[FFmpeg][Reader] Timeout expired";
    } else {
        error = "[FFmpeg][Reader]" + errorToString(resCode);
        _rtspState = errortoCameraState(resCode, _blockTimerExp);
    }
    /** @TODO: add callback **/
    Log::error(error.c_str());
}

sage::ImageFormat CameraFFmpeg::getImageFormat() {
    std::lock_guard<std::mutex> lck(_mutex);
    return imgFormat;
}

void CameraFFmpeg::setUrl(std::string url) {
    _url = url;
}

std::string CameraFFmpeg::getUrl() {
    return _url;
}

CameraFFmpeg::~CameraFFmpeg() {
    std::cout << "Start stopping" << std::endl;
    stop();
    std::cout << "Stopped" << std::endl;
}
