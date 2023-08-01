#include "camera_opencv.h"

CameraOpencv::CameraOpencv(std::string url, sage::OpencvType type)
    : _url(url),
      _capType(type),
      _isCapOpened(false) {
}

CameraOpencv::~CameraOpencv() {
}

std::string CameraOpencv::getName() {
    return "OpenCV";
}

bool CameraOpencv::start() {
    if (isStreaming()) {
        Log::critical("Can't start camera, already playing");
        return false;
    }
    _isStreaming.store(true);
    _camThread = std::thread(&CameraOpencv::mainLoop, this);
    return true;
}

bool CameraOpencv::stop() {
    if (_camThread.joinable()) {
        _isStreaming.store(false);
        _cap.release();
        _camThread.join();
    }
}

void CameraOpencv::selectCaptureType() {
    switch (_capType) {
        case sage::OpencvType::V4L: {
            _isCapOpened = _cap.open(_url, cv::CAP_V4L);
            break;
        }
        case sage::OpencvType::FFMPEG: {
            _isCapOpened = _cap.open(_url, cv::CAP_FFMPEG);
            break;
        }
        case sage::OpencvType::GSTREAMER: {
            _isCapOpened = _cap.open(_url, cv::CAP_GSTREAMER);
            break;
        }
        case sage::OpencvType::ANY: {
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

    int framerate = _cap.get(cv::CAP_PROP_FPS);
    int framecount = _cap.get(cv::CAP_PROP_FRAME_COUNT);
    int codec = static_cast<int>(_cap.get(cv::CAP_PROP_FOURCC));

    char ch_codec[] = {(char)(codec & 0XFF),
                       (char)((codec & 0XFF00) >> 8),
                       (char)((codec & 0XFF0000) >> 16),
                       (char)((codec & 0XFF000000) >> 24), 0};

    sage::ImageFormat cv_imgFormat = sage::ImageFormat::RAW;
    if (!strcmp(ch_codec, "FMP4")) {    
        cv_imgFormat = sage::ImageFormat::MPEG4;
    } else if (!strcmp(ch_codec, "h264")) {
        cv_imgFormat = sage::ImageFormat::H264;
    } else if (!strcmp(ch_codec, "YUYV")) {
        cv_imgFormat = sage::ImageFormat::YUYV;
    } 

    cv::Mat _frame;
    while (isStreaming()) {
        std::lock_guard<std::mutex> locker(_mutex);
        if (_cap.read(_frame)) {
            sage::swImage& image = _queue.next();
            image.setBytes(_frame.data,
                           _frame.cols * _frame.rows * _frame.elemSize());

            image->duration = (framecount > 0) ? (framecount / framerate) : 0;
            image->imgSize = sage::Size<int>(_frame.cols, _frame.rows);
            image->imgFormat = cv_imgFormat;
            image->imgColorType = sage::ColorType::BGR;
            image->channels = _frame.channels();
            triggerImage(image);
            if (framecount > 0) {
                usleep((1000 / framerate) * 1000);
            }
        } else {
            usleep(100000);
            continue;
        }
    }
}