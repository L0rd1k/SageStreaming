#pragma once

#include <string>
#include <unistd.h>
#include <thread>

#include "cameras/cameras_handler.h"
#include "image/image.h"
#include "definitions/local_definitions.h"

class CameraOpencv : public sage::CamerasHandler {
public:
    CameraOpencv(std::string url, sage::OpencvType type);
    virtual ~CameraOpencv();
    virtual bool start() override;
    virtual bool stop() override;
    virtual std::string getName() override;
    void selectCaptureType();
private:
    void mainLoop();
    cv::VideoCapture _cap;
    bool _isCapOpened;
    std::mutex _mutex;
    std::string _url;
    sage::OpencvType _capType;
    std::thread _camThread;
};