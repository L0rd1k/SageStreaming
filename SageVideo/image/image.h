#pragma once

#include <opencv2/opencv.hpp>

#include "data/data.h"
#include "definitions/local_definitions.h"
#include "utils/size.h"

namespace sage {

#pragma pack(push, 1)
struct Image : public DataHeader {
    Image() : DataHeader(DataType::Image),
              imgFormat(ImageFormat::Undefined),
              imgSize(sage::Size<int>(0, 0)),
              imgSourceType(ImageSource::Undefined) {
    }
    ImageFormat imgFormat;
    sage::Size<int> imgSize;
    ImageSource imgSourceType;
    ColorType imgColorType;
    uint8_t channels;
    int64_t duration;
};
#pragma pack(pop)

using swImage = DataCore<Image>;

static cv::Mat toMat(const sage::swImage& in) {
    int type = 0;
    if (in->channels == 1) {
        type = CV_8UC1;
    } else if (in->channels == 3) {
        type = CV_8UC3;
    } else if (in->channels == 4) {
        type = CV_8UC4;
    }
    cv::Mat mat(in->imgSize.height(), in->imgSize.width(), type, in.data());
    return mat;
}

static swImage fromMat(cv::Mat* in) {
    swImage result;
    result->imgSize = sage::Size<int>(in->size().width, in->size().height);
    result->channels = in->channels();
    result->imgFormat = sage::ImageFormat::RAW;
    if(in->channels() == 1) {
        result->imgColorType = sage::ColorType::GRAY;
    } else if(in->channels() == 3) {
        result->imgColorType = sage::ColorType::BGR;
    } else if(in->channels() == 4) {
        result->imgColorType = sage::ColorType::BGRA;
    }
    result.setBytes(in->data, in->dataend - in->datastart);
    return result;
}

}  // namespace sage