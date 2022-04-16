#pragma once

#include "definitions/LocalDefinitions.h"
#include "data/Data.h"
#include "utils/Size.h"

namespace img {

#pragma pack(push, 1)
struct Image : public DataHeader {
    Image() : DataHeader(DataType::Image),
    imgFormat(ImageFormat::Undefined),
    imgSize(um::Size<int>(0,0)),
    imgSourceType(ImageSource::Undefined) {

    }
    ImageFormat imgFormat;
    um::Size<int> imgSize;
    ImageSource imgSourceType;

};
#pragma pack(pop)


using swImage = DataCore<Image>;
}