#pragma once

#include <gst/gst.h>
#include <gst/pbutils/missing-plugins.h>
#include <sys/stat.h>
#include <math.h>

#include "gstreamer_safe_ptr.h"
#include "utils/log.h"

#define VERSION_NUM(major, minor, micro) \
    (major * 1000000 + minor * 1000 + micro)
#define FULL_GST_VERSION \
    VERSION_NUM(GST_VERSION_MAJOR, GST_VERSION_MINOR, GST_VERSION_MICRO)
#define COLOR_ELEM "videoconvert"
#define COLOR_ELEM_NAME COLOR_ELEM
#define SAGE_GST_FORMAT(format) (format)

/*!
 * \brief handleMessage
 * Handles gstreamer bus messages. Mainly for debugging purposes and ensuring
 * clean shutdown on error
 */
static void handleMessage(GstElement* pipeline) {
    sage::gstrmr::GSafePtr<GstBus> bus;
    GstStreamStatusType tp;
    GstElement* elem = NULL;

    bus.attach(gst_element_get_bus(pipeline));

    while (gst_bus_have_pending(bus)) {
        sage::gstrmr::GSafePtr<GstMessage> msg;
        msg.attach(gst_bus_pop(bus));
        if (!msg || !GST_IS_MESSAGE(msg.get())) continue;

        if (gst_is_missing_plugin_message(msg)) {
            Log::warning(
                "your GStreamer installation is missing a required plugin");
        } else {
            switch (GST_MESSAGE_TYPE(msg)) {
                case GST_MESSAGE_STATE_CHANGED:
                    GstState oldstate, newstate, pendstate;
                    gst_message_parse_state_changed(msg, &oldstate, &newstate,
                                                    &pendstate);
                    break;
                case GST_MESSAGE_ERROR: {
                    sage::gstrmr::GSafePtr<GError> err;
                    sage::gstrmr::GSafePtr<gchar> debug;
                    gst_message_parse_error(msg, err.getRef(), debug.getRef());
                    sage::gstrmr::GSafePtr<gchar> name;
                    name.attach(gst_element_get_name(GST_MESSAGE_SRC(msg)));
                    Log::warning("Embedded video playback halted; module ",
                                 name.get(), " reported: ",
                                 (err ? err->message : "<unknown reason>"));
                    Log::debug(NULL, "GStreamer debug: ", debug.get());

                    gst_element_set_state(GST_ELEMENT(pipeline),
                                          GST_STATE_NULL);
                    break;
                }
                case GST_MESSAGE_EOS:
                    break;
                case GST_MESSAGE_STREAM_STATUS:
                    gst_message_parse_stream_status(msg, &tp, &elem);
                    break;
                default:
                    break;
            }
        }
    }
}

inline static bool is_gst_element_exists(const std::string& name) {
    sage::gstrmr::GSafePtr<GstElementFactory> testfac;
    testfac.attach(gst_element_factory_find(name.c_str()));
    return (bool)testfac;
}

static bool isPathExist(const std::string& path) {
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

static void toFraction(const double decimal, int& numerator_i,
                       int& denominator_i) {
    double err = 1.0;
    int denominator = 1;
    double numerator = 0;
    for (int check_denominator = 1;; check_denominator++) {
        double check_numerator = (double)check_denominator * decimal;
        double dummy;
        double check_err = modf(check_numerator, &dummy);
        if (check_err < err) {
            err = check_err;
            denominator = check_denominator;
            numerator = check_numerator;
            if (err < FLT_EPSILON) break;
        }
        if (check_denominator == 100)  // limit
            break;
    }
    numerator_i = round(numerator);
    denominator_i = denominator;
    // printf("%g: %d/%d    (err=%g)\n", decimal, numerator_i, denominator_i,
    // err);
}