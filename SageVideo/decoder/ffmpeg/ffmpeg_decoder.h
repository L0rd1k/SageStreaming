#pragma once

#include "decoder/decoder.h"
#include "ffmpeg/FFmpegUtils.h"
#include "utils/Log.h"

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavutil/pixdesc.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
}

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

namespace sage {

class FFmpegDecoder : public sage::Decoder {
public:
    FFmpegDecoder();
    virtual ~FFmpegDecoder();
    virtual bool decode(const img::swImage& in, img::swImage& out) override;
private:
    int selectDecoderType(const img::swImage& img);
    void preprocessDecoder(int codec, int width, int height);
    void resetDecoderInfo();
    void initSwsContext(int format, int width, int height);
    void resetSwsContext();

private:
    AVCodecContext* _codecCtx = nullptr;
    
    // const AVCodec* _codec;
    // AVCodec* _codec = nullptr;

    AVFrame* _frame = nullptr;
    AVPacket* _packet = nullptr;
    AVCodecParserContext *_parser = nullptr;

    // std::unique_ptr<AVPacket> _packet = nullptr;
    SwsContext* _swsCtx = nullptr;
    bool _isInited;
    int _swsFormat = 0;
    int _swsWidth = 0;
    int _swsHeight = 0;
};

}  // namespace sage
