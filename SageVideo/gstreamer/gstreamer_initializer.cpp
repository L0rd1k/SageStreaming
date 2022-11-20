#include "gstreamer_initializer.h"

gstrmr::GstInitializer& gstrmr::GstInitializer::init() {
    static gstrmr::GstInitializer g_init;
    if (g_init.isFailed)
        Log::error("Can't initialize GStreamer");
    return g_init;
}

gstrmr::GstInitializer::GstInitializer() 
: isFailed(false) {
    call_deinit = false;

    sage::gstrmr::GSafePtr<GError> err;
    gboolean gst_init_res = gst_init_check(NULL, NULL, err.getRef());
    if (!gst_init_res) {
        Log::warning("Can't initialize GStreamer: ", err ? err->message : "<unknown reason>");
        isFailed = true;
        return;
    }
    guint major, minor, micro, nano;
    gst_version(&major, &minor, &micro, &nano);
    if (GST_VERSION_MAJOR != major) {
        Log::warning("incompatible GStreamer version");
        isFailed = true;
        return;
    }
}


gstrmr::GstInitializer::~GstInitializer() {
    if (call_deinit) {
        // Debug leaks: GST_LEAKS_TRACER_STACK_TRACE=1
        // GST_DEBUG="GST_TRACER:7" GST_TRACERS="leaks"
        gst_deinit();
    }
}