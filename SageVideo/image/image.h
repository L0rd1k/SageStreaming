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
};
#pragma pack(pop)

using swImage = DataCore<Image>;

static cv::Mat cv_toMat(const sage::swImage& img) {
    int type = 0;
    if (img->channels == 1) {
        type = CV_8UC1;
    } else if (img->channels == 3) {
        type = CV_8UC3;
    } else if (img->channels == 4) {
        type = CV_8UC4;
    }
    cv::Mat mat(img->imgSize.height(), img->imgSize.width(), type, img.data());
    return mat;
}

}  // namespace sage