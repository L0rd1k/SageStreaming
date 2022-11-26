#include "camera_gstreamer.h"

GstreamerCamera::GstreamerCamera(const std::string& url) : _url(url) {}

GstreamerCamera::~GstreamerCamera() {}

bool GstreamerCamera::start() {
    if (isStreaming()) {
        Log::critical("Can't start camera, already playing");
        return false;
    }
    _isStreaming = true;
    _gstCap = std::make_unique<gstrmr::GStreamerCapture>();
    if (!_gstCap) {
        Log::critical("Gstreamer capture wasn't inited");
        return false;
    } else {
        if (!_gstCap->open(_url)) {
            Log::critical("Failed opening url");
        }
    }
    _camThread = std::thread(&GstreamerCamera::mainLoop, this);
    return true;
}

bool GstreamerCamera::stop() {}

void GstreamerCamera::mainLoop() {
    while (isStreaming()) {
        std::lock_guard<std::mutex> locker(_mutex);
        sage::swImage& image = _queue.next();
        if(_gstCap->grabFrame(image)) {
            if(_gstCap->retrieveFrame(image)) {
                image->type = DataType::Image;
                triggerImage(image);
            }
        }
    }
}