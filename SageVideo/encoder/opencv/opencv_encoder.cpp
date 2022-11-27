#include "opencv_encoder.h"

const std::string sage::toString(sage::EncoderExt ext) {
    switch (ext) {
        case sage::EncoderExt::JPEG:
            return ".jpeg";
        case sage::EncoderExt::PNG:
            return ".png";
    }
}

sage::OpencvEncoder::OpencvEncoder() {}

sage::OpencvEncoder::~OpencvEncoder() {}

bool sage::OpencvEncoder::encoder(const sage::swImage& in, sage::swImage& out) {
    cv::Mat srcImg = sage::toMat(in);
    std::vector<uchar> buffer; 
    std::vector<int32_t> params = {cv::IMWRITE_JPEG_QUALITY, 100};
    cv::imencode(toString(sage::EncoderExt::JPEG), srcImg, buffer, params);
    out.setBytes(buffer.data(), buffer.size());
    out->imgSize = sage::Size<int>(srcImg.cols, srcImg.rows);
    out->imgFormat = sage::ImageFormat::JPEG;
}