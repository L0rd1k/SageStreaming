#pragma once

#include "decoder/decoder.h"
#include "ffmpeg/FFmpegUtils.h"
#include "utils/Log.h"

extern "C" {
    #include "libavcodec/avcodec.h"
    #include "libswscale/swscale.h"
    #include "libavutil/pixdesc.h"
}

class FFmpegDecoder : public sage::Decoder {
public:
    FFmpegDecoder();
    virtual ~FFmpegDecoder();
    virtual bool decode(const img::swImage& in, img::swImage& out) override;
private:
    int selectDecoderType(const img::swImage& img);
    void preprocessDecoder(int width, int height, int codec);
    void resetDecoderInfo();
    void initSwsContext(int width, int height, int format);
    void resetSwsContext();
private:
    AVCodecContext* _codecCtx = nullptr;
    AVCodec* _codec = nullptr;
    AVFrame* _frame = nullptr;
    AVPacket* _packet = nullptr;
    SwsContext* _swsCtx = nullptr;
    bool _isInited;
    int _swsFormat = 0;
    int _swsWidth = 0;
    int _swsHeight = 0;

};
