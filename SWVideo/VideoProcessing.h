#pragma once

#ifdef __linux__
extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavformat/avio.h>
    #include <libavutil/opt.h>
    #include <libavutil/avutil.h>
    #include "libavutil/pixfmt.h"
    #include "libavutil/log.h"
    #include "libavutil/old_pix_fmts.h"
    #include <libswscale/swscale.h>
}
#endif
#include <iostream>

class VideoProcessing {
public:
    VideoProcessing();
    virtual ~VideoProcessing();
//private:
//    int readVideoStream(const char* filename);
//    void saveFrame(AVFrame *pFrame, int width, int height, int iFrame);
//    bool openContext(); 
//private:
//    AVFormatContext *_context = NULL;
//    uint video_stream = -1;
//
//    int i, videoStream;
//    AVCodecContext *pCodecCtxOrig = NULL;
//    AVCodecContext *pCodecCtx = NULL;
//    AVCodec *pCodec = NULL;
//    AVFrame *pFrame = NULL;
//    AVFrame *pFrameRGB = NULL;
//    AVPacket packet;
//    int frameFinished;
//    int numBytes;
//    uint8_t *buffer = NULL;
//    struct SwsContext *sws_ctx = NULL;
//    const char* filename; 
};