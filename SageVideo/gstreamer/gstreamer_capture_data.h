#pragma once 

#include "gstreamer_definitions.h"
#include "gstreamer_safe_ptr.h"

namespace gstrmr {

struct GstCaptureData {
    sage::gstrmr::GSafePtr<GstElement> pipeline;
    sage::gstrmr::GSafePtr<GstElement> v4l2src;
    sage::gstrmr::GSafePtr<GstElement> sink;
    sage::gstrmr::GSafePtr<GstSample> sample;
    sage::gstrmr::GSafePtr<GstCaps> caps;

    gint64 duration;
    gint width;
    gint height;
    double fps;
    bool isPosFramesSupported;
    bool isPosFramesEmulated;
    gint64 emulatedFrameNumber;
    int hw_device;
};

}