#pragma once

#include <string>
#include <atomic>
#include <unistd.h>

#include "ffmpeg_utils.h"
#include "utils/elapsed_timer.h"
#include "utils/log.h"
#include "utils/size.h"
#include "image/image.h"
#include "image/image_queue.h"
#include "definitions/local_definitions.h"
#include "cameras/cameras_handler.h"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

class CameraFFmpeg : public sage::CamerasHandler {
public:
    CameraFFmpeg(std::string url, RtspTransportType type = RtspTransportType::Indefined);
    virtual ~CameraFFmpeg();
    virtual bool start() override;
    virtual bool stop() override;
    bool prepareContext();
    bool openContext();
    bool closeContext();
    bool receiveContext();
    bool blockingTimerExpired();
    void handleError(int resCode);
    bool handleVideoFrame(AVStream* stream, AVPacket* packet);
    TrimmedAVPacket trimAVPacket(AVPacket* packet);
    void mainLoop();
    void setUrl(std::string url);
    void logStreamInfo();    
    std::string getUrl();  
    sage::ImageFormat getImageFormat();
private:
    void performFpsDelay(AVStream* stream, AVPacket* packet);
    sage::ImageFormat imgFormat;
    std::string _url;
    std::thread _camThread;
    std::mutex _mutex;
    RtspCameraState _rtspState;
    RtspTransportType _rtspTransportType;
    AVFormatContext* _context = nullptr;
    AVStream* _videoStream = nullptr;
    AVStream* _audioStream = nullptr;
    bool _blockTimerExp;
    ElapsedTimer _blockTimer;
    int _blockTimerTimeout;
    int _previousPacket;
};