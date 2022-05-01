#include "FFmpegDecoder.h"

FFmpegDecoder::FFmpegDecoder() : _isInited(false) {
    initializeFFmpeg();
}

FFmpegDecoder::~FFmpegDecoder() {
    resetDecoderInfo();
    resetSwsContext();
}

int FFmpegDecoder::selectDecoderType(const img::swImage& img) {
    switch (img->imgFormat) {
        case img::ImageFormat::H264:
            return AVCodecID::AV_CODEC_ID_H264;
        case img::ImageFormat::JPEG:
            return AVCodecID::AV_CODEC_ID_MJPEG;
        default:
            return AVCodecID::AV_CODEC_ID_NONE;
    }
    return 0;
}

void FFmpegDecoder::resetDecoderInfo() {
    if (_frame) {
        av_frame_unref(_frame);
        av_frame_free(&_frame);
    }
    if (_packet) {
        delete _packet;
    }
    if (_codecCtx) {
        avcodec_close(_codecCtx);
        avcodec_free_context(&_codecCtx);
    }
}

void FFmpegDecoder::preprocessDecoder(int width, int height, int codec) {
    if (!_codecCtx && _codecCtx->codec_id == static_cast<AVCodecID>(codec)) {
        return;
    }
    resetDecoderInfo();
    _codec = avcodec_find_decoder(static_cast<AVCodecID>(codec));
    if (_codec) {
        _codecCtx = avcodec_alloc_context3(_codec);
        _codecCtx->width = width;
        _codecCtx->height = height;
        if (_codecCtx) {
            int flag = avcodec_open2(_codecCtx, _codec, nullptr);
            if (flag < 0) {
                Log() << "[FFMPEG]Can't open codec [avcodec_open2]!";
                return;
            }
            _packet = new AVPacket;
            _frame = av_frame_alloc();
            if (_packet && _frame && flag >= 0) {
                _isInited = true;
            }
        }
    }
}

void FFmpegDecoder::initSwsContext(int width, int height, int format) {
    if (_swsCtx != nullptr && _swsFormat == format &&
        _swsWidth == width && _swsHeight == height) {
        return;
    }
    resetSwsContext();
    AVPixelFormat formatFrom = static_cast<AVPixelFormat>(format);
    AVPixelFormat formatTo = AV_PIX_FMT_BGR24;
    _swsCtx = sws_getContext(width, height,
                             formatFrom,
                             width, height,
                             formatTo, SWS_BICUBIC,
                             NULL, NULL, NULL);
    _swsFormat = format;
    _swsWidth = width;
    _swsHeight = height;
}

void FFmpegDecoder::resetSwsContext() {
    if (_swsCtx)
        sws_freeContext(_swsCtx);
}

bool FFmpegDecoder::decode(const img::swImage& in, img::swImage& out) {
    if (int codec = selectDecoderType(in)) {
        preprocessDecoder(codec, in->imgSize.width(), in->imgSize.height());
    }
    if (_isInited) {
        av_init_packet(_packet);
        _packet->data = in.data();
        _packet->size = in.dataSize();
        int frameReceived = -1;
        if (avcodec_decode_video2(_codecCtx, _frame, &frameReceived, _packet) >= 0) {
            if (frameReceived) {
                int width = _codecCtx->width;
                int height = _codecCtx->height;
                int format = _codecCtx->pix_fmt;
                initSwsContext(format, width, height);
                int size = avpicture_get_size(AV_PIX_FMT_RGB24, width, height);
                out.memAllocate(size);
                uint8_t* outData[1] = {out.data()};
                int outLineSize[1] = { 3 * width };
                sws_scale(_swsCtx, _frame->data, _frame->linesize, 0, height, outData, outLineSize);  
                
                out->imgFormat = img::ImageFormat::RAW;
                out->imgSize = um::Size<int>(width, height);
                out->channels = 3;
                out->imgColorType = img::ColorType::BGR;
                return true;
            }
        }
    }
    return false;
}