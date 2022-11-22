#pragma once 

#include <mutex>

#include "cameras/cameras_handler.h"

#include "reader/gstreamer_capture.h"

class GstreamerCamera : public sage::CamerasHandler {
public:
    GstreamerCamera(const std::string& url);
    virtual ~GstreamerCamera();
    virtual bool start() override;
    virtual bool stop() override;
private:
    void mainLoop();
    std::thread _camThread;
    std::unique_ptr<gstrmr::GStreamerCapture> _gstCap;
    std::string _url;
    std::mutex _mutex;

};