#pragma once

#include "data/Data.h"
#include "utils/Size.h"

namespace img {

#pragma pack(push, 1)
struct Image : public DataHeader {
    Image();
    img::ImageFormat imgFormat;
    um::Size<int> imgSize;
    ImageSource imgSourceType;

};
#pragma pack(pop)

using swImage = DataCore<Image>;
}