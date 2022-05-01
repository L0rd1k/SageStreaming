#pragma once

#include "image/Image.h"

class Decoder {
public: 
    Decoder();
    virtual ~Decoder();
    virtual bool decode(const img::swImage& in, img::swImage& out) = 0;
};