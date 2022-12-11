#pragma once

#include "encoder/encoder.h"

namespace sage {

enum class EncoderExt {
    JPEG = 0,
    PNG = 1,
    Undefined = 2,
};

const std::string toString(EncoderExt ext);

class OpencvEncoder : public Encoder {
public:
    OpencvEncoder();
    virtual ~OpencvEncoder();
    virtual bool encode(const sage::swImage& in, sage::swImage& out) override;
};

}  // namespace sage