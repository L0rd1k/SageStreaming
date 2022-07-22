#include "camera_ffmpeg.h"

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

CameraFFmpeg::CameraFFmpeg(std::string url, RtspTransportType type)
    : sage::CamerasHandler(),
      _url(url),
      _rtspTransportType(type),
      _blockTimerExp(false),
      _previousPacket(0) {
    initializeFFmpeg();
}

bool CameraFFmpeg::start() {
    if (isPlaying()) {
        Log() << "Can't start camera, already playing";
        return false;
    }
    _isStreaming = true;
    _camThread = std::thread(&CameraFFmpeg::mainLoop, this);
    return true;
}

bool CameraFFmpeg::stop() {
    if (_camThread.joinable()) {
        if (_context) {
            /** @warning Custom method from prebuild FFMPEG 2.8 **/
            avformat_preclose_input(&_context);
        }
        _isStreaming = false;
        _camThread.join();
    }
}

bool CameraFFmpeg::isPlaying() {
    return _isStreaming;
}

void CameraFFmpeg::mainLoop() {
    if (prepareContext() && openContext()) {
        Log() << "[FFMPEG] Prepared main loop";
        while (isStreaming() && receiveContext()) {
            _blockTimer.restart();
        }
    }
    if (!isPlaying()) {
        _rtspState = RtspCameraState::Stopped;
    }
    closeContext();
}

bool CameraFFmpeg::receiveContext() {
    AVPacket packet;
    av_init_packet(&packet);
    int code = av_read_frame(_context, &packet);
    if (code != 0) {
        handleError(code);
        return false;
    }

    if (_videoStream && packet.stream_index == _videoStream->index) {
        handleVideoFrame(_videoStream, &packet);
    }

    av_free_packet(&packet);
    av_init_packet(&packet);
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
    imgFormat = img::ImageFormat::Undefined;
    if (stream->codec->codec_id == AV_CODEC_ID_MJPEG) {
        // Log::trace("AV_CODEC_ID_MJPEG");
        imgFormat = img::ImageFormat::JPEG;
    } else if (stream->codec->codec_id == AV_CODEC_ID_H264) {
        // Log::trace("AV_CODEC_ID_H264");
        imgFormat = img::ImageFormat::H264;
    } else if (stream->codec->codec_id == AV_CODEC_ID_RAWVIDEO) {
        // Log::trace("AV_CODEC_ID_RAWVIDEO");
        imgFormat = img::ImageFormat::RAW;
    } else if (stream->codec->codec_id == AV_CODEC_ID_MPEG4) {
        // Log::trace("AV_CODEC_ID_MPEG4");
        imgFormat = img::ImageFormat::MPEG4;
    } else {
        Log() << "[FFMPEG] Unsupported image format: " + std::to_string(stream->codec->codec_id);
        return false;
    }

    if (!packet->data || !packet->size) {
        return false;
    }

    if (_rtspTransportType == RtspTransportType::Vid) {
        performFpsDelay(stream, packet);
    }

    img::swImage& image = _queue.next();

    if (imgFormat == img::ImageFormat::JPEG) {
        TrimmedAVPacket trimmed = trimAVPacket(packet);
        Log::trace(trimmed.size);
        image.setBytes(trimmed.data, trimmed.size);
    } else {
        image.setBytes(packet->data, packet->size);
    }

    image->imgFormat = imgFormat;
    image->imgSourceType = img::ImageSource::RTSP;
    image->imgSize = um::Size<int>(stream->codec->width,
                                   stream->codec->height);

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
    return !_isStreaming || _blockTimerExp;
}

bool CameraFFmpeg::prepareContext() {
    Log() << "[FFMPEG] prepare context";
    _isStreaming = true;
    _rtspState = RtspCameraState::Undefined;

    _context = avformat_alloc_context();
    _context->interrupt_callback.callback = blockingOperationCallback;
    _context->interrupt_callback.opaque = this;
    _blockTimer.restart();
    _blockTimerTimeout = cam::ffmpeg::timeoutConnect;
    _blockTimerExp = false;

    AVDictionary* dict = nullptr;
    if (_rtspTransportType == RtspTransportType::Tcp) {
        // Log() << "[FFMPEG] TCP";
        av_dict_set(&dict, "rtps_transport", "tcp", 0);
    } else if (_rtspTransportType == RtspTransportType::Udp) {
        // Log() << "[FFMPEG] UDP";
        av_dict_set(&dict, "rtps_transport", "udp", 0);
    } else if (_rtspTransportType == RtspTransportType::V4l) {
        // Log() << "[FFMPEG] V4L";
        av_dict_set(&dict, "framerate", "25", 0);
    } else if (_rtspTransportType == RtspTransportType::Vid) {
        // Log() << "[FFMPEG] VID";
        av_dict_set(&dict, "framerate", "25", 0);
    } else {
        Log::error("[FFMpeg] Unknown transport type");
        return false;
    }

    int resCode = avformat_open_input(&_context, _url.c_str(), NULL, &dict);
    if (resCode != 0 || _blockTimerExp) {
        Log::error("[FFMPEG] Video input error found:", _url.c_str());
        handleError(resCode);
        return false;
    } 
    return true;
}

void CameraFFmpeg::logStreamInfo() {
    Log::info("\n Extension:", _context->iformat->extensions, "\n",
              "Format name:", _context->iformat->name, "\n",
              "Bitrate:", _context->bit_rate, "\n",
              "Duratrion:", _context->duration, "\n",
              "Codec id:", _context->streams[0]->codec->codec_id, "\n",
              "Channels:", _context->streams[0]->codec->channels);
}

bool CameraFFmpeg::openContext() {
    // Get input stream information
    int code = avformat_find_stream_info(_context, NULL);
    logStreamInfo();

    if (code != 0 || _blockTimerExp) {
        Log::error("[FFMPEG] OpenContext Error");
        handleError(code);
        return false;
    }

    for (uint i = 0; i < _context->nb_streams; i++) {
        if (_context->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            // Log::trace("[FFMPEG] AVMEDIA_TYPE_VIDEO:", i);
            _videoStream = _context->streams[i];
        } else if (_context->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
            // Log() << "[FFMPEG] AVMEDIA_TYPE_AUDIO";
            _audioStream = _context->streams[i];
        } else if (_context->streams[i]->codec->codec_type == AVMEDIA_TYPE_DATA) {
            // Log() << "[FFMPEG] AVMEDIA_TYPE_DATA";
        }
    }
    _blockTimerTimeout = cam::ffmpeg::timeoutStream;
    return true;
}

bool CameraFFmpeg::closeContext() {
    _blockTimerTimeout = cam::ffmpeg::timeoutTeardown;
    _blockTimer.restart();
    avformat_close_input(&_context);
    avformat_free_context(_context);
    _context = NULL;
    _previousPacket = 0;
}

void CameraFFmpeg::handleError(int resCode) {
    std::string error;
    if (_blockTimerExp) {
        error = "[FFMpeg] Timeout expired";
    } else {
        error = "[FFMpeg]" + errorToString(resCode);
        _rtspState = errortoCameraState(resCode, _blockTimerExp);
    }
    /** @TODO: add callback **/
    Log::error(error.c_str());
}

img::ImageFormat CameraFFmpeg::getImageFormat() {
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
    stop();
}
