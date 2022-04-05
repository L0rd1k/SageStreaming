#include "CameraFFmpeg.h"

CameraFFmpeg::CameraFFmpeg(std::string url) :
_url(url),
_isStreaming(false), 
_rtspTransportType(RtspTransportType::Udp),
_blockTimerExp(false), 
_previousPacket(0),
_buffer(1000) {
    initializeFFmpeg();
}

bool CameraFFmpeg::start() {
    if(isPlaying()) {
        Log() << "Can't start camera, already playing"; 
        return false;
    }
    _isStreaming = true;
    _camThread = std::thread(&CameraFFmpeg::mainLoop, this);
    return true;
}

bool CameraFFmpeg::stop() {
    if(_camThread.joinable()) {
        _isStreaming = false;
        _camThread.join();
    }
}

bool CameraFFmpeg::isPlaying() {
    return _isStreaming;
} 

void CameraFFmpeg::mainLoop() {
    if(prepareContext() && openContext()) {
        while(_isStreaming && receiveContext()) {
            _blockTimer.restart();
        }
    }
    if(!isPlaying()) {
        _rtspState = RtspCameraState::Stopped;
    }
    closeContext();
}

bool CameraFFmpeg::receiveContext() {
    AVPacket packet;
    av_init_packet(&packet);
    int code = av_read_frame(_context, &packet);
    if(code != 0) {
        handleError(code);
        return false;
    }

    if(_videoStream && packet.stream_index == _videoStream->index) {
        handleVideoFrame(_videoStream, &packet);
    }

    av_free_packet(&packet);
    av_init_packet(&packet);
    return true;
}

bool CameraFFmpeg::handleVideoFrame(AVStream* stream, AVPacket* packet) {
    std::lock_guard<std::mutex> locker(_mutex);
    img::ImageFormat imgFormat = img::ImageFormat::Undefined;
    if(stream->codec->codec_id == AV_CODEC_ID_MJPEG) {
        imgFormat = img::ImageFormat::JPEG;        
    } else if (stream->codec->codec_id == AV_CODEC_ID_H264) {
        imgFormat = img::ImageFormat::H264;
    } else {
        Log() << "[FFMPEG] Unsupported image format" + std::to_string(stream->codec->codec_id);
        return false;
    }

    if(!packet->data || !packet->size) {
        return false;
    }
    _previousPacket = packet->pts;
    
    img::swImage& image = _buffer.next();

    if(imgFormat == img::ImageFormat::JPEG) {
        TrimmedAVPacket trimmed = trimAVPacket(packet);
        image.setBytes(trimmed.data, trimmed.size);
    } else {
        image.setBytes(packet->data, packet->size);
    }

    image->imgFormat = imgFormat;
    image->imgSourceType = img::ImageSource::RTSP;
    image->imgSize = um::Size<int>(stream->codec->width,
                                   stream->codec->height);

    return true;
}

TrimmedAVPacket CameraFFmpeg::trimAVPacket(AVPacket* packet) {
    int startOffset = 0;
    int endOffset = 0;
    for(int i = 0; i < packet->size - 1; i++) {
        uint8_t curr = packet->data[i];
        uint8_t next = packet->data[i + 1];
        if(curr == 0xFF && next == 0xD8) {
            startOffset = i;
        } 
        if(curr == 0xFF && next == 0xD9) { // jpeg end sequence
            endOffset = i;
            break;
        }
        TrimmedAVPacket trimmed;
        trimmed.data = packet->data + startOffset;
        trimmed.size = endOffset - startOffset + 2;
        return trimmed;
    }
}


int blockingOperationCallback(void *sender) {
   CameraFFmpeg* ffmpegClient = (CameraFFmpeg*)sender;
   return ffmpegClient->blockingTimerExpired();    
}

bool CameraFFmpeg::blockingTimerExpired() {
    _blockTimerExp = _blockTimer.expired(_blockTimerExp);
    return !_isStreaming || _blockTimerExp;
}

bool CameraFFmpeg::prepareContext() {
    _isStreaming = true;
    _rtspState = RtspCameraState::Undefined;

    _context = avformat_alloc_context();
    _context->interrupt_callback.callback = blockingOperationCallback;
    _context->interrupt_callback.opaque = this; 
    _blockTimer.restart();
    _blockTimerTimeout = cam::ffmpeg::timeoutConnect;
    _blockTimerExp = false;

    AVDictionary *dict = nullptr;
    if(_rtspTransportType == RtspTransportType::Tcp) {
        av_dict_set(&dict, "rtps_transport", "tcp", 0);    
    } else if (_rtspTransportType == RtspTransportType::Udp) {
        av_dict_set(&dict, "rtps_transport", "udp", 0);   
    } else {
        Log() << "[FFMpeg] Unknown transport type";
        return false;
    }

    int resCode = avformat_open_input(&_context, _url.c_str(), NULL, &dict);
    if (resCode != 0 || _blockTimerExp) {
        handleError(resCode);
        return false;
    }
    return true;
}

bool CameraFFmpeg::openContext() {
    int code = avformat_find_stream_info(_context, NULL);
    if(code != 0 || _blockTimerExp) {
        handleError(code);
        return false;
    }
    for(uint i = 0; i < _context->nb_streams; i++) {
        if(_context->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            _videoStream = _context->streams[i];
        } else if (_context->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
            _audioStream = _context->streams[i];
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
    if(_blockTimerExp) {
        error = "[FFMpeg] Timeout expired";
    } else {
        error = "[FFMpeg]" + errorToString(resCode); 
        _rtspState = errortoCameraState(resCode, _blockTimerExp);
    }
    /** @TODO: add callback **/
    Log() << error;
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
