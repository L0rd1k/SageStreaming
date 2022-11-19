#pragma once

#include <gst/gst.h>

#include "gstreamer_safe_ptr.h"
#include "gstreamer_definitions.h"

namespace gstrmr {

class GStreamerCapture final {
private:
    sage::gstrmr::GSafePtr<GstElement> pipeline;
    sage::gstrmr::GSafePtr<GstElement> v4l2src;
    sage::gstrmr::GSafePtr<GstElement> sink;
    sage::gstrmr::GSafePtr<GstSample> sample;
    sage::gstrmr::GSafePtr<GstCaps> caps;

    gint videoStream;
    gint audioStream;
    gint64 duration;
    gint width;
    gint height;
    double fps;
    bool isPosFramesSupported;
    bool isPosFramesEmulated;
    gint64 emulatedFrameNumber;
    gint outputAudioFormat;
    gint audioBaseIndex;
    gint nAudioChannels;
    gint audioSamplesPerSecond;

    // Mat audioFrame;
    // VideoAccelerationType va_type;

    int hw_device;

public:
    GStreamerCapture();
    virtual ~GStreamerCapture();
    virtual bool grabFrame();
    // virtual bool retrieveFrame(int /*unused*/, OutputArray dst);
    // bool grabAudioFrame();
    // bool retrieveVideoFrame(int /*unused*/, OutputArray dst);
    // bool retrieveAudioFrame(int /*unused*/, OutputArray dst);
    virtual double getProperty(int propId) const;
    virtual bool setProperty(int propId, double value);
    virtual bool isOpened() const { return (bool)pipeline; }
    // bool open(int id, const cv::VideoCaptureParameters &params);
    // bool open(const String &filename_,
    //           const cv::VideoCaptureParameters &params);
    // static void newPad(GstElement * /*elem*/, GstPad *pad, gpointer data);
    // bool configureHW(const cv::VideoCaptureParameters &);
    // bool configureStreams(const cv::VideoCaptureParameters &);
    // bool setAudioProperties(const cv::VideoCaptureParameters &);

protected:
    bool isPipelinePlaying();
    void startPipeline();
    void stopPipeline();
    void restartPipeline();
    void setFilter(const char *prop, int type, int v1, int v2);
    void removeFilter(const char *filter);
};

} // namespace gstrmr