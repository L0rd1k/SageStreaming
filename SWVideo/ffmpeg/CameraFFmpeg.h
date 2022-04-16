#pragma once

#include <string>
#include <atomic>

#include "FFmpegUtils.h"
#include "utils/ElapsedTimer.h"
#include "utils/Log.h"
#include "utils/Size.h"
#include "image/Image.h"
#include "image/ImageQueue.h"
#include "definitions/LocalDefinitions.h"
#include "cameras/CamerasHandler.h"

class CameraFFmpeg : public CamerasHandler {
public:
    CameraFFmpeg(std::string url);
    virtual ~CameraFFmpeg();
    virtual bool start() override;
    virtual bool stop() override;
    bool isPlaying();
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
    std::string getUrl();  
protected:
    ImageQueue _buffer;
private:
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