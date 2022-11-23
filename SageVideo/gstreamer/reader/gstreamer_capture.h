#pragma once

#include <gst/gst.h>
#include <gst/gstelement.h>
#include <gst/video/video.h>
#include <gst/app/gstappsink.h>
#include <gst/app/gstappsrc.h>

#include <memory>
#include <sstream>
#include <string>

#include "gstreamer_safe_ptr.h"
#include "gstreamer_definitions.h"
#include "gstreamer_capture_data.h"
#include "gstreamer_initializer.h"
#include "gstreamer_capture_prop.h"

#include "utils/log.h"
#include "image/image.h"

namespace gstrmr {
    
class GStreamerCapture final {
public:
    GStreamerCapture();
    virtual ~GStreamerCapture();
    virtual bool grabFrame(sage::swImage& img);
    virtual bool retrieveFrame(sage::swImage& img);
    bool retrieveVideoFrame(sage::swImage& img, bool& isBufferExist);
    virtual double getProperty(int propId) const;
    virtual bool setProperty(int propId, double value);
    virtual bool isOpened() const;
    bool open(int id);
    bool open(const std::string &filename_);
    static void newPad(GstElement * /*elem*/, GstPad *pad, gpointer data);
protected:
    bool isPipelinePlaying();
    bool startPipeline();
    void stopPipeline();
    void restartPipeline();
    void setFilter(const char *prop, int type, int v1, int v2);
    void removeFilter(const char *filter);
private:
    std::shared_ptr<gstrmr::GstCaptureData> _gstData;
};

} // namespace gstrmr