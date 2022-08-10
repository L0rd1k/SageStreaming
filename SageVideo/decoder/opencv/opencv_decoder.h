#pragma once

#include "decoder/decoder.h"
#include "utils/log.h"

namespace sage {

class OpencvDecoder : public sage::Decoder {
    OpencvDecoder();
    virtual ~OpencvDecoder();
    virtual bool decode(const sage::swImage& in, sage::swImage& out) override;
};

}  // namespace sage
