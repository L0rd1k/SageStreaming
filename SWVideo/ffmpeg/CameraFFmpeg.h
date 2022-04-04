#pragma once

#include <string>
#include <atomic>

#include "FFmpegUtils.h"
#include "utils/ElapsedTimer.h"
#include "utils/Log.h"
#include "image/Image.h"
#include "definitions/LocalDefinitions.h"

class CameraFFmpeg {
public:
    CameraFFmpeg(std::string url);
    virtual ~CameraFFmpeg();
    bool start();
    bool stop();
    bool isPlaying();
    bool prepareContext();
    bool openContext();
    bool receiveContext();
    bool blockingTimerExpired();
    void handleError(int resCode);
    bool handleVideoFrame(AVStream* stream, AVPacket* packet);
    void mainLoop();
    void setUrl(std::string url);
    std::string getUrl();    
private:

    std::string _url;
    std::atomic<bool> _isStreaming;
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

};