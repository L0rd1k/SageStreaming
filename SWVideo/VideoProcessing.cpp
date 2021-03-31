#include "VideoProcessing.h"

VideoProcessing::VideoProcessing() {
    filename = "/home/ilya/Projects/GitProjects/ffmpeg-video-player/testVideo.mp4";
    readVideoStream(filename);
}

void VideoProcessing::saveFrame(AVFrame *pFrame, int width, int height, int iFrame) {
    FILE *pFile;
    char szFilename[32];
    int  y;
    // Open file
    sprintf(szFilename, "frame%d.ppm", iFrame);
    pFile=fopen(szFilename, "wb");
    if(pFile==NULL)
        return;
    // Write header
    fprintf(pFile, "P6\n%d %d\n255\n", width, height);

    // Write pixel data
    for(y=0; y<height; y++)
        fwrite(pFrame->data[0]+y*pFrame->linesize[0], 1, width*3, pFile);
    // Close file
    fclose(pFile);
}

bool VideoProcessing::openContext() {
    std::cout << "...Open video file!\n";
    if(avformat_open_input(&_context, filename, NULL, NULL)!=0) {
        std::cout << "Can't open video file!\n";
        return -1;
    }

    std::cout << "...Retrieve stream information!\n";
    int resCode = avformat_find_stream_info(_context, NULL);
    if (resCode != 0) {
        std::cout << "Couldn't find stream information!\n";
        return -1; 
    }

    std::cout << "...Dump information about file onto standard error!\n";
    // av_dump_format(_context, 0, filename, 0);

    std::cout << "...Search video stream!\n";
    video_stream =-1;
    for(uint i = 0; i < _context->nb_streams; i++) {
        if(_context->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_stream = i ;
            break;
        }
    }

    if(video_stream == -1) {
        std::cout << "Didn't find a video stream!\n";
        return -1;
    }

}

int VideoProcessing::readVideoStream(const char* filename) {
    av_register_all();
    openContext();

    std::cout << "...Get a pointer to the codec context for the video stream\n";
    pCodecCtxOrig = _context->streams[video_stream]->codec;

    std::cout << "...Find Find the decoder for the video stream\n";
    pCodec=avcodec_find_decoder(pCodecCtxOrig->codec_id);
    if(pCodec==NULL) {
        fprintf(stderr, "Unsupported codec!\n");
        return -1; 
    }


    std::cout << "...Copying codec context\n";
    pCodecCtx = avcodec_alloc_context3(pCodec);
    if(avcodec_copy_context(pCodecCtx, pCodecCtxOrig) != 0) {
        fprintf(stderr, "Couldn't copy codec context");
        return -1;
    }

    std::cout << "...Open codec\n";
    if(avcodec_open2(pCodecCtx, pCodec, NULL)<0)
        return -1;  
    

    std::cout << "...Allocate video frame\n";
    pFrame = av_frame_alloc();

    std::cout << "...Allocate an AVFrame structure\n";
    pFrameRGB = av_frame_alloc();
    if(pFrameRGB == NULL)
        return -1;

    std::cout << "...Determine required buffer size and allocate buffer\n";
    numBytes = avpicture_get_size(PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height);
    buffer = (uint8_t *)av_malloc(numBytes*sizeof(uint8_t));


    std::cout << "...Assign appropriate parts of buffer to image planes in pFrameRGB\n";
    avpicture_fill((AVPicture *)pFrameRGB, buffer, PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height);


    std::cout << "...Initialize SWS context for software scaling" << std::endl;
    sws_ctx = sws_getContext(pCodecCtx->width,
			   pCodecCtx->height,
			   pCodecCtx->pix_fmt,
			   pCodecCtx->width,
			   pCodecCtx->height,
			   PIX_FMT_RGB24,
			   SWS_BILINEAR,
			   NULL,
			   NULL,
			   NULL
			   );

    int frameCounter = 0;
    while(av_read_frame(_context, &packet)>=0) {
        if(packet.stream_index == video_stream) {
            avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);
            if(frameFinished) {
	            sws_scale(sws_ctx, (uint8_t const * const *)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameRGB->data, pFrameRGB->linesize);
	            if(++frameCounter <= 5) {
	                saveFrame(pFrameRGB, pCodecCtx->width, pCodecCtx->height, frameCounter);
                }
            }
        }
        av_free_packet(&packet);
    }
}

VideoProcessing::~VideoProcessing() {
    av_free(buffer);
    av_frame_free(&pFrameRGB);
    av_frame_free(&pFrame);
    avcodec_close(pCodecCtx);
    avcodec_close(pCodecCtxOrig);
    avformat_close_input(&_context);
}