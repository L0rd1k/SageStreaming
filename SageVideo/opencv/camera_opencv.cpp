#include "camera_opencv.h"

CameraOpencv::CameraOpencv(std::string url, sage::CVCapType type)
    : _url(url),
      _capType(type),
      _isCapOpened(false) {
}

CameraOpencv::~CameraOpencv() {
}

bool CameraOpencv::start() {
    if (isStreaming()) {
        Log::critical("Can't start camera, already playing");
        return false;
    }
    _isStreaming = true;
    _camThread = std::thread(&CameraOpencv::mainLoop, this);
    return true;
}

bool CameraOpencv::stop() {
    if (_camThread.joinable()) {
        _isStreaming = false;
        _cap.release();
        _camThread.join();
    }
}

void CameraOpencv::selectCaptureType() {
    switch (_capType) {
        case sage::CVCapType::V4L: {
            _isCapOpened = _cap.open(_url, cv::CAP_V4L);
            break;
        }
        case sage::CVCapType::FFMPEG: {
            _isCapOpened = _cap.open(_url, cv::CAP_FFMPEG);
            break;
        }
        case sage::CVCapType::GSTREAMER: {
            _isCapOpened = _cap.open(_url, cv::CAP_GSTREAMER);
            break;
        }
        case sage::CVCapType::ANY: {
            _isCapOpened = _cap.open(_url, cv::CAP_ANY);
            break;
        }
        default: {
            _isCapOpened = _cap.open(_url);
        }
    }
}

void CameraOpencv::mainLoop() {
    selectCaptureType();
    if (!_isCapOpened) {
        Log::critical("Video stream was not captured:", _url);
    }

    cv::Mat _frame;
    while (isStreaming()) {
        std::lock_guard<std::mutex> locker(_mutex);
        if (_cap.read(_frame)) {
            sage::swImage& image = _queue.next();
            image.setBytes(_frame.data,
                           _frame.cols * _frame.rows * _frame.elemSize());

            image->imgSize = sage::Size<int>(_frame.cols, _frame.rows);
            image->imgFormat = sage::ImageFormat::RAW;
            image->imgColorType = sage::ColorType::BGR;
            image->imgSourceType = sage::ImageSource::RTSP;
            image->channels = _frame.channels();

            triggerImage(image);
        } else {
            usleep(100000);
            continue;
        }
    }
}