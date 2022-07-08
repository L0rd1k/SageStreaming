#pragma once

#include "image/Image.h"

namespace sage {

class Decoder {
public:
    Decoder();
    virtual ~Decoder();
    virtual bool decode(const img::swImage& in, img::swImage& out) = 0;
};

}  // namespace sage