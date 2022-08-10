#include "opencv_decoder.h"

sage::OpencvDecoder::OpencvDecoder() {
}

sage::OpencvDecoder::~OpencvDecoder() {
}

bool sage::OpencvDecoder::decode(const sage::swImage& in, sage::swImage& out) {
    if(in->imgFormat == sage::ImageFormat::RAW) {
        out = in;
        return true;
    } else if (in->imgFormat != sage::ImageFormat::JPEG) {
        Log::warning("[Opencv][Decoder] Support only JPEG codec.");
        return false;
    }
    cv::Mat original(1, in.dataSize(), CV_8UC1, in.data());
    cv::Mat decoded = cv::imdecode(original, cv::IMREAD_COLOR);
    out = sage::fromMat(&decoded);
    out->imgSize = in->imgSize;
    out->imgFormat = sage::ImageFormat::RAW;
    return true;
}