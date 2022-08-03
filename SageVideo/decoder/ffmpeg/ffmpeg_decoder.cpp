#include "ffmpeg_decoder.h"

sage::FFmpegDecoder::FFmpegDecoder()
    : _isInited(false) {
    initializeFFmpeg();
}

sage::FFmpegDecoder::~FFmpegDecoder() {
    resetDecoderInfo();
    resetSwsContext();
}

int sage::FFmpegDecoder::selectDecoderType(const sage::swImage& img) {
    switch (img->imgFormat) {
        case sage::ImageFormat::H264: {
            Log::info("[FFmpeg][Decoder] H264");
            return AVCodecID::AV_CODEC_ID_H264;
        }
        case sage::ImageFormat::JPEG: {
            Log::info("[FFmpeg][Decoder] JPEG");
            return AVCodecID::AV_CODEC_ID_MJPEG;
        }
        case sage::ImageFormat::MPEG4: {
            Log::info("[FFmpeg][Decoder] MPEG4");
            return AVCodecID::AV_CODEC_ID_MPEG4;
        }
        default: {
            Log::info("[FFmpeg][Decoder] None");
            return AVCodecID::AV_CODEC_ID_NONE;
        }
    }
    return 0;
}

void sage::FFmpegDecoder::resetDecoderInfo() {
    if (_frame) {
        av_frame_unref(_frame);
        av_frame_free(&_frame);
        av_packet_unref(_packet);
        av_packet_free(&_packet);
    }
    if (_packet) {
        delete _packet;
        _packet = nullptr;
    }
    if (_codecCtx) {
        avcodec_close(_codecCtx);
        avcodec_free_context(&_codecCtx);
    }
}

void sage::FFmpegDecoder::preprocessDecoder(int codec, int width, int height) {
    if (_codecCtx && _codecCtx->codec_id == static_cast<AVCodecID>(codec)) {
        return;
    }
    resetDecoderInfo();
    Log::trace("[FFmpeg][Decoder]Intialize ffmpeg decoder:", avcodec_get_name(AVCodecID(codec)), width, "x", height);
    /** Find appropriate decoder. **/
    const AVCodec* _codec = avcodec_find_decoder(static_cast<AVCodecID>(codec));
    if (_codec) {
        _codecCtx = avcodec_alloc_context3(_codec);
        _codecCtx->thread_count = 0;
        /** Width and height MUST be initialized here because this
         * information is not available in the bitstream. **/
        _codecCtx->width = width;
        _codecCtx->height = height;
        if (_codecCtx) {
            int flag = avcodec_open2(_codecCtx, _codec, nullptr);
            if (flag < 0) {
                Log::critical("[FFmpeg][Decoder]Can't open codec [avcodec_open2]!");
                return;
            }
            // _packet = av_packet_alloc();
            _packet = new AVPacket();
            _frame = av_frame_alloc();
            if (!_frame) {
                Log::critical("[FFmpeg][Decoder]Can't allocate video frame[av_frame_alloc]!");
                return;
            }
            if (_packet && _frame && flag >= 0) {
                _isInited = true;
            }
        }
    } else {
        Log::error("[FFmpeg][Decoder] Codec not found");
    }
}

void sage::FFmpegDecoder::initSwsContext(int format, int width, int height) {
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

void sage::FFmpegDecoder::resetSwsContext() {
    if (_swsCtx)
        sws_freeContext(_swsCtx);
}

bool sage::FFmpegDecoder::decode(const sage::swImage& in, sage::swImage& out) {
    if (int codecType = selectDecoderType(in)) {
        Log::critical(codecType, in->imgSize.width(), in->imgSize.height());
        preprocessDecoder(codecType, in->imgSize.width(), in->imgSize.height());
    }

    if (_isInited) {
        av_init_packet(_packet);

        _packet->data = in.data();
        _packet->size = in.dataSize();

        Log::trace(_packet->size);

        int frameReceived = -1;
        int ret = avcodec_send_packet(_codecCtx, _packet);
        if (ret < 0) {
            if (ret == AVERROR_EOF)
                Log::error("[Decoder] avcodec_send_packet() return value is AVERROR_EOF.");
            else if (ret == AVERROR_INVALIDDATA)
                Log::error("[Decoder] avcodec_send_packet() INVALID DATA!");
            else
                Log::error("[Decoder] avcodec_send_packet() return value is negative:", ret);
            return false;
        }

        while (ret >= 0) {
            ret = avcodec_receive_frame(_codecCtx, _frame);
            if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
                return false;
            else if (ret < 0) {
                fprintf(stderr, "Error during decoding\n");
                exit(1);
            }

            int width = _codecCtx->width;
            int height = _codecCtx->height;
            int format = _codecCtx->pix_fmt;

            initSwsContext(format, width, height);
            int size = av_image_get_buffer_size(AV_PIX_FMT_RGB24, width, height, 1);
            out.memAllocate(size);
            uint8_t* outData[1] = {out.data()};
            int outLineSize[1] = {3 * width};
            sws_scale(_swsCtx, _frame->data, _frame->linesize, 0, height, outData, outLineSize);

            out->imgFormat = sage::ImageFormat::RAW;
            out->imgSize = sage::Size<int>(width, height);
            out->imgColorType = sage::ColorType::BGR;

            return true;
        }
    }
    return false;
}