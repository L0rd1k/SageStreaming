#pragma once

#include <string>

namespace gstrmr {

enum class GstCaptureProp {
    GST_CAP_PROP_POS_MSEC,
    GST_CAP_PROP_POS_FRAMES,
    GST_CAP_PROP_POS_AVI_RATIO,
    GST_CAP_PROP_FRAME_WIDTH,
    GST_CAP_PROP_FRAME_HEIGHT,
    GST_CAP_PROP_FPS,
    GST_CAP_PROP_FRAME_COUNT,
    GST_CAP_PROP_BRIGHTNESS,
    GST_CAP_PROP_CONTRAST,
    GST_CAP_PROP_SATURATION,
    GST_CAP_PROP_HUE,
    GST_CAP_PROP_HW_ACCELERATION,
    GST_CAP_PROP_HW_DEVICE,
    GST_CAP_GSTREAMER_QUEUE_LENGTH,
    GST_CAP_PROP_GAIN,
    GST_CAP_PROP_CONVERT_RGB,
};

inline static std::string get_gst_propname(int propId) {
    switch (static_cast<GstCaptureProp>(propId)) {
        case gstrmr::GstCaptureProp::GST_CAP_PROP_BRIGHTNESS:
            return "brightness";
        case gstrmr::GstCaptureProp::GST_CAP_PROP_CONTRAST:
            return "contrast";
        case gstrmr::GstCaptureProp::GST_CAP_PROP_SATURATION:
            return "saturation";
        case gstrmr::GstCaptureProp::GST_CAP_PROP_HUE:
            return "hue";
        default:
            return std::string();
    }
}

}  // namespace gstrmr