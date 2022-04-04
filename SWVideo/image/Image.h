#pragma once

#include "data/Data.h"

namespace img {

#pragma pack(push, 1)
struct Image : public DataHeader {
    Image();
};
#pragma pack(pop)

using swImage = DataCore<Image>;

}