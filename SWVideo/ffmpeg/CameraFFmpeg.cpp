#include "CameraFFmpeg.h"

CameraFFmpeg::CameraFFmpeg(std::string url) :
_url(url),
_isStreaming(false), 
_rtspTransportType(RtspTransportType::Udp),
_blockTimerExp(false) {
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
            
        }
    }
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
}

bool CameraFFmpeg::handleVideoFrame(AVStream* stream, AVPacket* packet) {
    std::lock_guard<std::mutex> locker(_mutex);
    ImageFomat imgFormat = ImageFomat::Undefined;
    if(stream->codec->codec_id == AV_CODEC_ID_MJPEG) {
        imgFormat = ImageFomat::JPEG;        
    } else if (stream->codec->codec_id == AV_CODEC_ID_H264) {
        imgFormat = ImageFomat::H264;
    } else {
        Log() << "[FFMPEG] Unsupported image format" + std::to_string(stream->codec->codec_id);
        return false;
    }

    if(!packet->data || !packet->size) {
        return false;
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
}
