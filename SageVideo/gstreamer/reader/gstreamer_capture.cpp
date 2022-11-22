#include "gstreamer_capture.h"

#include <gst/gst.h>
#include <gst/gstelement.h>
#include <gst/video/video.h>
#include <gst/app/gstappsink.h>
#include <gst/app/gstappsrc.h>

gstrmr::GStreamerCapture::GStreamerCapture() {
    _gstData = std::make_shared<gstrmr::GstCaptureData>();
    _gstData->duration = -1;
    _gstData->width = -1;
    _gstData->height = -1;
    _gstData->fps = -1;
    _gstData->isPosFramesSupported = false;
    _gstData->isPosFramesEmulated = false;
    _gstData->emulatedFrameNumber = -1;
    _gstData->hw_device = -1;
}

bool gstrmr::GStreamerCapture::isOpened() const {
    return (bool)_gstData->pipeline;
}

/*!
 * \brief CvCapture_GStreamer::close
 * Closes the pipeline and destroys all instances
 */
gstrmr::GStreamerCapture::~GStreamerCapture() {
    if (isPipelinePlaying()) stopPipeline();
    if (_gstData->pipeline && GST_IS_ELEMENT(_gstData->pipeline.get())) {
        gst_element_set_state(_gstData->pipeline, GST_STATE_NULL);
        _gstData->pipeline.release();
    }
}

/*!
 * \brief CvCapture_GStreamer::grabFrame
 * \return
 * Grabs a sample from the pipeline, awaiting consumation by retreiveFrame.
 * The pipeline is started if it was not running yet
 */
bool gstrmr::GStreamerCapture::grabFrame(sage::swImage& img) {
    if (!_gstData->pipeline || !GST_IS_ELEMENT(_gstData->pipeline.get())) {
        return false;
    }
    // start the pipeline if it was not in playing state yet
    if (!this->isPipelinePlaying()) {
        if (!this->startPipeline()) {
            return false;
        }
    }

    // bail out if EOS
    if (gst_app_sink_is_eos(GST_APP_SINK(_gstData->sink.get()))) return false;
    _gstData->sample.attach(
        gst_app_sink_pull_sample(GST_APP_SINK(_gstData->sink.get())));
    if (!_gstData->sample) return false;
    if (_gstData->isPosFramesEmulated) _gstData->emulatedFrameNumber++;

    bool isBufferExist = false;
    if (!retrieveVideoFrame(img, isBufferExist)) {
        return false;
    }
    return true;
}

bool gstrmr::GStreamerCapture::retrieveVideoFrame(sage::swImage& img,
                                                  bool& isBufferExist) {
    GstCaps* frame_caps =
        gst_sample_get_caps(_gstData->sample);  // no lifetime transfer
    if (!frame_caps) {
        Log::error(NULL, "GStreamer: gst_sample_get_caps() returns NULL");
        return false;
    }
    if (!GST_CAPS_IS_SIMPLE(frame_caps)) {
        // bail out in no caps
        Log::error("GStreamer: GST_CAPS_IS_SIMPLE(frame_caps) check is failed");
        return false;
    }

    GstVideoInfo info = {};
    gboolean video_info_res = gst_video_info_from_caps(&info, frame_caps);
    if (!video_info_res) {
        Log::error(
            "GStreamer: gst_video_info_from_caps() is failed. Can't handle "
            "unknown layout");
    }

    int frame_width = GST_VIDEO_INFO_WIDTH(&info);
    int frame_height = GST_VIDEO_INFO_HEIGHT(&info);
    if (frame_width <= 0 || frame_height <= 0) {
        Log::error("GStreamer: Can't query frame size from GStreamer sample");
        return false;
    }

    GstStructure* structure =
        gst_caps_get_structure(frame_caps, 0);  // no lifetime transfer
    if (!structure) {
        Log::error(
            "GStreamer: Can't query 'structure'-0 from GStreamer sample");
        return false;
    }

    const gchar* name_ = gst_structure_get_name(structure);
    if (!name_) {
        Log::error("GStreamer: Can't query 'name' from GStreamer sample");
        return false;
    }
    std::string name(name_);
    // we support these types of data:
    //     video/x-raw, format=BGR                      -> 8bit, 3 channels
    //     video/x-raw, format=GRAY8                    -> 8bit, 1 channel
    //     video/x-raw, format=UYVY                     -> 8bit, 2 channel
    //     video/x-raw, format=YUY2                     -> 8bit, 2 channel
    //     video/x-raw, format=YVYU                     -> 8bit, 2 channel
    //     video/x-raw, format=NV12                     -> 8bit, 1 channel
    //     (height is 1.5x larger than true height) video/x-raw, format=NV21 ->
    //     8bit, 1 channel (height is 1.5x larger than true height) video/x-raw,
    //     format=YV12                     -> 8bit, 1 channel (height is 1.5x
    //     larger than true height) video/x-raw, format=I420 -> 8bit, 1 channel
    //     (height is 1.5x larger than true height) video/x-bayer -> 8bit, 1
    //     channel image/jpeg                                   -> 8bit, mjpeg:
    //     buffer_size x 1 x 1 video/x-raw, format=GRAY16_LE (BE)           ->
    //     16 bit, 1 channel video/x-raw, format={BGRA, RGBA, BGRx, RGBx} ->
    //     8bit, 4 channels
    // bayer data is never decoded, the user is responsible for that

    if (name == "video/x-raw") {
        const gchar* format_ = gst_structure_get_string(structure, "format");
        if (!format_) {
            Log::error("GStreamer: Can't query 'format' of 'video/x-raw'");
            return false;
        }
        std::string format(format_);
        img->imgFormat = sage::ImageFormat::RAW;
        if (format == "BGR") {
            img->imgColorType = sage::ColorType::BGR;
            img->channels = 3;
        } else if (format == "GRAY8") {
            img->imgColorType = sage::ColorType::GRAY;
            img->channels = 1;
        } else if (format == "BGRA") {
            img->imgColorType = sage::ColorType::BGRA;
            img->channels = 4;
        } else if (format == "RGBA") {
            img->imgColorType = sage::ColorType::RGBA;
            img->channels = 4;
        } else if (format == "BGRX") {
            img->imgColorType = sage::ColorType::BGRX;
            img->channels = 4;
        } else if (format == "RGBX") {
            img->imgColorType = sage::ColorType::RGBX;
            img->channels = 4;
        } else if (format == "UYVY") {
            img->imgColorType = sage::ColorType::UYVY;
            img->channels = 2;
        } else if (format == "YUY2") {
            img->imgColorType = sage::ColorType::YUY2;
            img->channels = 2;
        } else if (format == "YVYU") {
            img->imgColorType = sage::ColorType::YVYU;
            img->channels = 2;
        } else {
            Log::error("Unsupported GStreamer 'video/x-raw' format: %s",
                       format.c_str());
        }
    } else if (name == "video/x-bayer") {
        img->imgFormat = sage::ImageFormat::BAYER;
        img->channels = 1;
    } else if (name == "image/jpeg") {
        img->imgFormat = sage::ImageFormat::JPEG;
        img->channels = 1;
        isBufferExist = true;
    } else {
        Log::error("Unsupported GStreamer layer type: %s", name.c_str());
        return false;
    }
    return true;
}

bool gstrmr::GStreamerCapture::retrieveFrame(sage::swImage& img) {
    if (!_gstData->sample) {
        return false;
    }
    GstBuffer* buffer = gst_sample_get_buffer(_gstData->sample);
    if (!buffer) {
        return false;
    }
    GstMapInfo info{};
    if (!gst_buffer_map(buffer, &info, GST_MAP_READ)) {
        Log::error(
            "GStreamer: Failed to map GStreamer buffer to system memory");
        return false;
    }
    try {
       img.memAllocate(info.size);
       memcpy(img.data(), info.data, info.size);
    } catch (...) {
        gst_buffer_unmap(buffer, &info);
        throw;
    }
    gst_buffer_unmap(buffer, &info);
    return true;
}

bool gstrmr::GStreamerCapture::isPipelinePlaying() {
    if (!_gstData->pipeline || !GST_IS_ELEMENT(_gstData->pipeline.get())) {
        Log::warning("GStreamer: pipeline have not been created");
        return false;
    }
    GstState current, pending;
    GstClockTime timeout = 5 * GST_SECOND;
    GstStateChangeReturn ret =
        gst_element_get_state(_gstData->pipeline, &current, &pending, timeout);
    if (!ret) {
        Log::warning("unable to query pipeline state");
        return false;
    }
    return current == GST_STATE_PLAYING;
}

/*!
 * \brief CvCapture_GStreamer::startPipeline
 * Start the pipeline by setting it to the playing state
 */
bool gstrmr::GStreamerCapture::startPipeline() {
    if (!_gstData->pipeline || !GST_IS_ELEMENT(_gstData->pipeline.get())) {
        Log::warning("GStreamer: pipeline have not been created");
        return false;
    }
    GstStateChangeReturn status =
        gst_element_set_state(_gstData->pipeline, GST_STATE_PLAYING);
    if (status == GST_STATE_CHANGE_ASYNC) {
        // wait for status update
        status = gst_element_get_state(_gstData->pipeline, NULL, NULL,
                                       GST_CLOCK_TIME_NONE);
    }
    if (status == GST_STATE_CHANGE_FAILURE) {
        handleMessage(_gstData->pipeline);
        _gstData->pipeline.release();
        Log::warning("unable to start pipeline");
        return false;
    }

    if (_gstData->isPosFramesEmulated) {
        _gstData->emulatedFrameNumber = 0;
    }

    handleMessage(_gstData->pipeline);
    return true;
}

void gstrmr::GStreamerCapture::stopPipeline() {
    if (!_gstData->pipeline || !GST_IS_ELEMENT(_gstData->pipeline.get())) {
        Log::warning("GStreamer: pipeline have not been created");
        return;
    }
    if (gst_element_set_state(_gstData->pipeline, GST_STATE_NULL) ==
        GST_STATE_CHANGE_FAILURE) {
        Log::warning("unable to stop pipeline");
        _gstData->pipeline.release();
    }
}

/*!
 * \brief CvCapture_GStreamer::restartPipeline
 * Restart the pipeline
 */
void gstrmr::GStreamerCapture::restartPipeline() {
    handleMessage(_gstData->pipeline);
    this->stopPipeline();
    this->startPipeline();
}

/*!
 * \brief CvCapture_GStreamer::setFilter
 * \param prop the property name
 * \param type glib property type
 * \param v1 the value
 * \param v2 second value of property type requires it, else NULL
 * Filter the output formats by setting appsink caps properties
 */
void gstrmr::GStreamerCapture::setFilter(const char* prop, int type, int v1,
                                         int v2) {
    if (!_gstData->caps || !(GST_IS_CAPS(_gstData->caps.get()))) {
        if (type == G_TYPE_INT) {
            _gstData->caps.attach(gst_caps_new_simple("video/x-raw", "format",
                                                      G_TYPE_STRING, "BGR",
                                                      prop, type, v1, NULL));
        } else {
            _gstData->caps.attach(
                gst_caps_new_simple("video/x-raw", "format", G_TYPE_STRING,
                                    "BGR", prop, type, v1, v2, NULL));
        }
    } else {
        if (!gst_caps_is_writable(_gstData->caps.get()))
            _gstData->caps.attach(
                gst_caps_make_writable(_gstData->caps.detach()));
        if (type == G_TYPE_INT) {
            gst_caps_set_simple(_gstData->caps, prop, type, v1, NULL);
        } else {
            gst_caps_set_simple(_gstData->caps, prop, type, v1, v2, NULL);
        }
    }
    _gstData->caps.attach(gst_caps_fixate(_gstData->caps.detach()));
    gst_app_sink_set_caps(GST_APP_SINK(_gstData->sink.get()), _gstData->caps);
    GST_LOG("filtering with caps: %" GST_PTR_FORMAT, _gstData->caps.get());
}

/*!
 * \brief CvCapture_GStreamer::removeFilter
 * \param filter filter to remove
 * remove the specified filter from the appsink template caps
 */
void gstrmr::GStreamerCapture::removeFilter(const char* filter) {
    if (!_gstData->caps) return;
    if (!gst_caps_is_writable(_gstData->caps.get()))
        _gstData->caps.attach(gst_caps_make_writable(_gstData->caps.detach()));
    GstStructure* s =
        gst_caps_get_structure(_gstData->caps, 0);  // no lifetime transfer
    gst_structure_remove_field(s, filter);
    _gstData->caps.attach(gst_caps_fixate(_gstData->caps.detach()));
    gst_app_sink_set_caps(GST_APP_SINK(_gstData->sink.get()), _gstData->caps);
}

/*!
 * \brief CvCapture_GStreamer::newPad link dynamic padd
 * \param pad
 * \param data
 * decodebin creates pads based on stream information, which is not known
 * upfront on receiving the pad-added signal, we connect it to the colorspace
 * conversion element
 */
void gstrmr::GStreamerCapture::newPad(GstElement*, GstPad* pad, gpointer data) {
    sage::gstrmr::GSafePtr<GstPad> sinkpad;
    GstElement* color = (GstElement*)data;
    sinkpad.attach(gst_element_get_static_pad(color, "sink"));
    if (!sinkpad) {
        Log::warning("no pad named sink");
        return;
    }
    gst_pad_link(pad, sinkpad.get());
}

/*!
 * \brief Create GStreamer pipeline
 * \param filename Filename to open in case of CV_CAP_GSTREAMER_FILE
 * \return boolean. Specifies if opening was successful.
 *
 * In case of camera 'index', a pipeline is constructed as follows:
 *    v4l2src ! autoconvert ! appsink
 *
 *
 * The 'filename' parameter is not limited to filesystem paths, and may be one
 * of the following:
 *
 *  - a normal filesystem path:
 *        e.g. video.avi or /path/to/video.avi or C:\\video.avi
 *  - an uri:
 *        e.g. file:///path/to/video.avi or rtsp:///path/to/stream.asf
 *  - a gstreamer pipeline description:
 *        e.g. videotestsrc ! videoconvert ! appsink
 *        the appsink name should be either 'appsink0' (the default) or
 * 'opencvsink'
 *
 *  GStreamer will not drop frames if the grabbing interval larger than the
 * framerate period. To support dropping for live streams add appsink 'drop'
 * parameter into your custom pipeline.
 *
 *  The pipeline will only be started whenever the first frame is grabbed.
 * Setting pipeline properties is really slow if we need to restart the pipeline
 * over and over again.
 *
 */
bool gstrmr::GStreamerCapture::open(int id) {
    gstrmr::GstInitializer::init();

    if (!is_gst_element_exists("v4l2src")) return false;
    std::ostringstream desc;
    desc << "v4l2src device=/dev/video" << id << " ! " << COLOR_ELEM
         << " ! appsink drop=true";
    return open(desc.str());
}

bool gstrmr::GStreamerCapture::open(const std::string& filename_) {
    gstrmr::GstInitializer::init();
    const gchar* filename = filename_.c_str();

    bool file = false;
    bool manualpipeline = false;
    sage::gstrmr::GSafePtr<char> uri;
    sage::gstrmr::GSafePtr<GstElement> uridecodebin;
    sage::gstrmr::GSafePtr<GstElement> color;
    GstStateChangeReturn status;

    sage::gstrmr::GSafePtr<GstElement> convert;
    sage::gstrmr::GSafePtr<GstElement> resample;

    // test if we have a valid uri. If so, open it with an uridecodebin
    // else, we might have a file or a manual pipeline.
    // if gstreamer cannot parse the manual pipeline, we assume we were given
    // and ordinary file path.
    Log::debug("OpenCV | GStreamer: ", filename);
    if (!gst_uri_is_valid(filename)) {
        if (isPathExist(filename_)) {
            sage::gstrmr::GSafePtr<GError> err;
            uri.attach(gst_filename_to_uri(filename, err.getRef()));
            if (uri) {
                file = true;
            } else {
                Log::warning("Error opening file: ", filename, " (",
                             (err ? err->message : "<unknown reason>"), ")");
                return false;
            }
        } else {
            sage::gstrmr::GSafePtr<GError> err;
            uridecodebin.attach(gst_parse_launch(filename, err.getRef()));
            if (!uridecodebin) {
                Log::warning("Error opening bin: ",
                             (err ? err->message : "<unknown reason>"));
                return false;
            }
            manualpipeline = true;
        }
    } else {
        uri.attach(g_strdup(filename));
    }
    Log::info("GStreamer: mode - ", (file             ? "FILE"
                                     : manualpipeline ? "MANUAL"
                                                      : "URI"));
    bool element_from_uri = false;
    if (!uridecodebin) {
        // At this writing, the v4l2 element (and maybe others too) does not
        // support caps renegotiation. This means that we cannot use an
        // uridecodebin when dealing with v4l2, since setting capture
        // properties will not work. The solution (probably only until
        // gstreamer 1.2) is to make an element from uri when dealing with
        // v4l2.
        sage::gstrmr::GSafePtr<gchar> protocol_;
        protocol_.attach(gst_uri_get_protocol(uri));
        assert(protocol_);
        std::string protocol = protocol_.get();
        if (protocol == "v4l2") {
            uridecodebin.reset(
                gst_element_make_from_uri(GST_URI_SRC, uri.get(), "src", NULL));
            assert(uridecodebin);
            element_from_uri = true;
        } else {
            uridecodebin.reset(gst_element_factory_make("uridecodebin", NULL));
            assert(uridecodebin);
            g_object_set(G_OBJECT(uridecodebin.get()), "uri", uri.get(), NULL);
        }
        if (!uridecodebin) {
            Log::warning("Can not parse GStreamer URI bin");
            return false;
        }
    }

    if (manualpipeline) {
        GstIterator* it = gst_bin_iterate_elements(GST_BIN(uridecodebin.get()));
        gboolean done = false;
        GValue value = G_VALUE_INIT;
        while (!done) {
            GstElement* element = NULL;
            sage::gstrmr::GSafePtr<gchar> name;
            switch (gst_iterator_next(it, &value)) {
                case GST_ITERATOR_OK:
                    element = GST_ELEMENT(g_value_get_object(&value));
                    name.attach(gst_element_get_name(element));
                    if (name) {
                        if (strstr(name, "opencvsink") != NULL ||
                            strstr(name, "appsink") != NULL) {
                            _gstData->sink.attach(
                                GST_ELEMENT(gst_object_ref(element)));
                        } else if (strstr(name, COLOR_ELEM_NAME) != NULL) {
                            color.attach(GST_ELEMENT(gst_object_ref(element)));
                        } else if (strstr(name, "v4l") != NULL) {
                            _gstData->v4l2src.attach(
                                GST_ELEMENT(gst_object_ref(element)));
                        }
                        name.release();
                        done = _gstData->sink && color && _gstData->v4l2src;
                    }
                    g_value_unset(&value);
                    break;
                case GST_ITERATOR_RESYNC:
                    gst_iterator_resync(it);
                    break;
                case GST_ITERATOR_ERROR:
                case GST_ITERATOR_DONE:
                    done = TRUE;
                    break;
            }
        }
        gst_iterator_free(it);
        if (!_gstData->sink) {
            Log::warning("cannot find appsink in manual pipeline");
            return false;
        }
        _gstData->pipeline.swap(uridecodebin);
    } else {
        _gstData->pipeline.reset(gst_pipeline_new(NULL));
        assert(_gstData->pipeline);
        _gstData->sink.reset(gst_element_factory_make("appsink", NULL));
        assert(_gstData->sink);
        // videoconvert (in 0.10: ffmpegcolorspace, in 1.x autovideoconvert)
        // automatically selects the correct colorspace conversion based on
        // caps.
        color.reset(gst_element_factory_make(COLOR_ELEM, NULL));
        assert(color);
        gst_bin_add_many(GST_BIN(_gstData->pipeline.get()), uridecodebin.get(),
                         color.get(), _gstData->sink.get(), NULL);

        if (element_from_uri) {
            if (!gst_element_link(uridecodebin, color.get())) {
                Log::warning("GStreamer(video): cannot link color -> sink");
                _gstData->pipeline.release();
                return false;
            }
        } else {
            g_signal_connect(uridecodebin, "pad-added", G_CALLBACK(newPad),
                             color.get());
        }

        if (!gst_element_link(color.get(), _gstData->sink.get())) {
            Log::warning("GStreamer(video): cannot link color -> sink");
            _gstData->pipeline.release();
            return false;
        }
    }

    if (!manualpipeline || strstr(filename, " max-buffers=") == NULL) {
        // TODO: is 1 single buffer really high enough?
        gst_app_sink_set_max_buffers(GST_APP_SINK(_gstData->sink.get()), 1);
    }
    if (!manualpipeline) {
        gst_base_sink_set_sync(GST_BASE_SINK(_gstData->sink.get()), FALSE);
    }
    // do not emit signals: all calls will be synchronous and blocking
    gst_app_sink_set_emit_signals(GST_APP_SINK(_gstData->sink.get()), FALSE);

    _gstData->caps.attach(gst_caps_from_string(
        "video/x-raw, format=(string){BGR, GRAY8}; "
        "video/x-bayer,format=(string){rggb,bggr,grbg,gbrg}; image/jpeg"));

    if (manualpipeline) {
        sage::gstrmr::GSafePtr<GstCaps> peer_caps;
        sage::gstrmr::GSafePtr<GstPad> sink_pad;
        sink_pad.attach(gst_element_get_static_pad(_gstData->sink, "sink"));
        peer_caps.attach(gst_pad_peer_query_caps(sink_pad, NULL));
        if (!gst_caps_can_intersect(_gstData->caps, peer_caps)) {
            _gstData->caps.attach(gst_caps_from_string(
                "video/x-raw, "
                "format=(string){UYVY,YUY2,YVYU,NV12,NV21,YV12,I420,BGRA,RGBA,"
                "BGRx,RGBx,GRAY16_LE,GRAY16_BE}"));
            assert(_gstData->caps);
        }
    }

    gst_app_sink_set_caps(GST_APP_SINK(_gstData->sink.get()), _gstData->caps);
    _gstData->caps.release();

    {
        GST_DEBUG_BIN_TO_DOT_FILE(GST_BIN(_gstData->pipeline.get()),
                                  GST_DEBUG_GRAPH_SHOW_ALL, "pipeline-init");

        status =
            gst_element_set_state(GST_ELEMENT(_gstData->pipeline.get()),
                                  file ? GST_STATE_PAUSED : GST_STATE_PLAYING);
        if (status == GST_STATE_CHANGE_ASYNC) {
            // wait for status update
            status = gst_element_get_state(_gstData->pipeline, NULL, NULL,
                                           GST_CLOCK_TIME_NONE);
        }
        if (status == GST_STATE_CHANGE_FAILURE) {
            GST_DEBUG_BIN_TO_DOT_FILE(GST_BIN(_gstData->pipeline.get()),
                                      GST_DEBUG_GRAPH_SHOW_ALL,
                                      "pipeline-error");
            handleMessage(_gstData->pipeline);
            _gstData->pipeline.release();
            Log::warning("unable to start pipeline");
            return false;
        }

        sage::gstrmr::GSafePtr<GstPad> pad;
        pad.attach(gst_element_get_static_pad(_gstData->sink, "sink"));
        sage::gstrmr::GSafePtr<GstCaps> buffer_caps;
        buffer_caps.attach(gst_pad_get_current_caps(pad));

        GstFormat format;

        format = GST_FORMAT_DEFAULT;
        if (!gst_element_query_duration(_gstData->sink, format,
                                        &_gstData->duration)) {
            handleMessage(_gstData->pipeline);
            Log::warning("unable to query duration of stream");
            _gstData->duration = -1;
        }
        handleMessage(_gstData->pipeline);
        const GstStructure* structure =
            gst_caps_get_structure(buffer_caps, 0);  // no lifetime transfer
        if (!gst_structure_get_int(structure, "width", &_gstData->width) ||
            !gst_structure_get_int(structure, "height", &_gstData->height)) {
            Log::warning("cannot query video width/height");
        }

        gint num = 0, denom = 1;
        if (!gst_structure_get_fraction(structure, "framerate", &num, &denom)) {
            Log::warning("cannot query video fps");
        }

        _gstData->fps = (double)num / (double)denom;

        {
            GstFormat format_;
            gint64 value_ = -1;
            gboolean status_;
            format_ = GST_FORMAT_DEFAULT;
            status_ = gst_element_query_position(
                _gstData->sink, SAGE_GST_FORMAT(format_), &value_);
            if (!status_ || value_ != 0 || _gstData->duration < 0) {
                Log::warning("Cannot query video position: status=", status_,
                             ", value=", value_,
                             ", duration=", _gstData->duration);
                _gstData->isPosFramesSupported = false;
                _gstData->isPosFramesEmulated = true;
                _gstData->emulatedFrameNumber = 0;
            } else {
                _gstData->isPosFramesSupported = true;
            }
        }
        GST_DEBUG_BIN_TO_DOT_FILE(GST_BIN(_gstData->pipeline.get()),
                                  GST_DEBUG_GRAPH_SHOW_ALL, "pipeline");
    }
    return true;
}

/*!
 * \brief CvCapture_GStreamer::getProperty retrieve the requested property from
 * the pipeline \param propId requested property \return property value
 *
 * There are two ways the properties can be retrieved. For seek-based properties
 * we can query the pipeline. For frame-based properties, we use the caps of the
 * last receivef sample. This means that some properties are not available until
 * a first frame was received
 */
double gstrmr::GStreamerCapture::getProperty(int propId) const {
    GstFormat format;
    gint64 value;
    gboolean status;
    if (!_gstData->pipeline) {
        Log::warning("GStreamer: no pipeline");
        return 0;
    }
    switch (static_cast<GstCaptureProp>(propId)) {
        case GstCaptureProp::GST_CAP_PROP_POS_MSEC:
            format = GST_FORMAT_TIME;
            status = gst_element_query_position(
                _gstData->sink.get(), SAGE_GST_FORMAT(format), &value);
            if (!status) {
                handleMessage(_gstData->pipeline);
                Log::warning("GStreamer: unable to query position of stream");
                return 0;
            }
            return value * 1e-6;  // nano seconds to milli seconds
        case GstCaptureProp::GST_CAP_PROP_POS_FRAMES:
            if (!_gstData->isPosFramesSupported) {
                if (_gstData->isPosFramesEmulated)
                    return _gstData->emulatedFrameNumber;
                return 0;  // TODO getProperty() "unsupported" value should be
                           // changed
            }
            format = GST_FORMAT_DEFAULT;
            status = gst_element_query_position(
                _gstData->sink.get(), SAGE_GST_FORMAT(format), &value);
            if (!status) {
                handleMessage(_gstData->pipeline);
                Log::warning("GStreamer: unable to query position of stream");
                return 0;
            }
            return value;
        case GstCaptureProp::GST_CAP_PROP_POS_AVI_RATIO:
            format = GST_FORMAT_PERCENT;
            status = gst_element_query_position(
                _gstData->sink.get(), SAGE_GST_FORMAT(format), &value);
            if (!status) {
                handleMessage(_gstData->pipeline);
                Log::warning("GStreamer: unable to query position of stream");
                return 0;
            }
            return ((double)value) / GST_FORMAT_PERCENT_MAX;
        case GstCaptureProp::GST_CAP_PROP_FRAME_WIDTH:
            return _gstData->width;
        case GstCaptureProp::GST_CAP_PROP_FRAME_HEIGHT:
            return _gstData->height;
        case GstCaptureProp::GST_CAP_PROP_FPS:
            return _gstData->fps;
        case GstCaptureProp::GST_CAP_PROP_FRAME_COUNT:
            return _gstData->duration;
        case GstCaptureProp::GST_CAP_PROP_BRIGHTNESS:
        case GstCaptureProp::GST_CAP_PROP_CONTRAST:
        case GstCaptureProp::GST_CAP_PROP_SATURATION:
        case GstCaptureProp::GST_CAP_PROP_HUE:
            if (_gstData->v4l2src) {
                std::string propName = get_gst_propname(propId);
                if (!propName.empty()) {
                    gint32 val = 0;
                    g_object_get(G_OBJECT(_gstData->v4l2src.get()),
                                 propName.c_str(), &val, NULL);
                    return static_cast<double>(val);
                }
            }
            break;
        case GstCaptureProp::GST_CAP_GSTREAMER_QUEUE_LENGTH:
            if (!_gstData->sink) {
                Log::warning("there is no sink yet");
                return 0;
            }
            return gst_app_sink_get_max_buffers(
                GST_APP_SINK(_gstData->sink.get()));
        default:
            Log::warning("unhandled property: ", propId);
            break;
    }

    return 0;
}

/*!
 * \brief CvCapture_GStreamer::setProperty
 * \param propId
 * \param value
 * \return success
 * Sets the desired property id with val. If the pipeline is running,
 * it is briefly stopped and started again after the property was set
 */
bool gstrmr::GStreamerCapture::setProperty(int propId, double value) {
    const GstSeekFlags flags =
        (GstSeekFlags)(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_ACCURATE);

    if (!_gstData->pipeline) {
        Log::warning("no pipeline");
        return false;
    }

    bool wasPlaying = this->isPipelinePlaying();
    if (wasPlaying) this->stopPipeline();

    switch (static_cast<GstCaptureProp>(propId)) {
        case GstCaptureProp::GST_CAP_PROP_POS_MSEC:
            if (!gst_element_seek_simple(GST_ELEMENT(_gstData->pipeline.get()),
                                         GST_FORMAT_TIME, flags,
                                         (gint64)(value * GST_MSECOND))) {
                handleMessage(_gstData->pipeline);
                Log::warning("GStreamer: unable to seek");
            } else {
                if (_gstData->isPosFramesEmulated) {
                    if (value == 0) {
                        _gstData->emulatedFrameNumber = 0;
                        return true;
                    } else {
                        _gstData->isPosFramesEmulated =
                            false;  // reset frame counter emulation
                    }
                }
            }
            break;
        case GstCaptureProp::GST_CAP_PROP_POS_FRAMES: {
            if (!_gstData->isPosFramesSupported) {
                if (_gstData->isPosFramesEmulated) {
                    if (value == 0) {
                        restartPipeline();
                        _gstData->emulatedFrameNumber = 0;
                        return true;
                    }
                }
                return false;
                Log::warning("unable to seek");
            }
            if (!gst_element_seek_simple(GST_ELEMENT(_gstData->pipeline.get()),
                                         GST_FORMAT_DEFAULT, flags,
                                         (gint64)value)) {
                handleMessage(_gstData->pipeline);
                Log::warning("GStreamer: unable to seek");
                break;
            }
            // wait for status update
            gst_element_get_state(_gstData->pipeline, NULL, NULL,
                                  GST_CLOCK_TIME_NONE);
            return true;
        }
        case GstCaptureProp::GST_CAP_PROP_POS_AVI_RATIO:
            if (!gst_element_seek_simple(
                    GST_ELEMENT(_gstData->pipeline.get()), GST_FORMAT_PERCENT,
                    flags, (gint64)(value * GST_FORMAT_PERCENT_MAX))) {
                handleMessage(_gstData->pipeline);
                Log::warning("GStreamer: unable to seek");
            } else {
                if (_gstData->isPosFramesEmulated) {
                    if (value == 0) {
                        _gstData->emulatedFrameNumber = 0;
                        return true;
                    } else {
                        _gstData->isPosFramesEmulated =
                            false;  // reset frame counter emulation
                    }
                }
            }
            break;
        case GstCaptureProp::GST_CAP_PROP_FRAME_WIDTH:
            if (value > 0)
                setFilter("width", G_TYPE_INT, (int)value, 0);
            else
                removeFilter("width");
            break;
        case GstCaptureProp::GST_CAP_PROP_FRAME_HEIGHT:
            if (value > 0)
                setFilter("height", G_TYPE_INT, (int)value, 0);
            else
                removeFilter("height");
            break;
        case GstCaptureProp::GST_CAP_PROP_FPS:
            if (value > 0) {
                int num = 0, denom = 1;
                toFraction(value, num, denom);
                setFilter("framerate", GST_TYPE_FRACTION, value, denom);
            } else
                removeFilter("framerate");
            break;
        case GstCaptureProp::GST_CAP_PROP_BRIGHTNESS:
        case GstCaptureProp::GST_CAP_PROP_CONTRAST:
        case GstCaptureProp::GST_CAP_PROP_SATURATION:
        case GstCaptureProp::GST_CAP_PROP_HUE:
            if (_gstData->v4l2src) {
                std::string propName = get_gst_propname(propId);
                if (!propName.empty()) {
                    gint32 val = cv::saturate_cast<gint32>(value);
                    g_object_set(G_OBJECT(_gstData->v4l2src.get()),
                                 propName.c_str(), &val, NULL);
                    return true;
                }
            }
            return false;
        case GstCaptureProp::GST_CAP_PROP_GAIN:
        case GstCaptureProp::GST_CAP_PROP_CONVERT_RGB:
            break;
        case GstCaptureProp::GST_CAP_GSTREAMER_QUEUE_LENGTH: {
            if (!_gstData->sink) {
                Log::warning("there is no sink yet");
                return false;
            }
            gst_app_sink_set_max_buffers(GST_APP_SINK(_gstData->sink.get()),
                                         (guint)value);
            return true;
        }
        default:
            Log::warning("GStreamer: unhandled property");
    }

    if (wasPlaying) this->startPipeline();

    return false;
}
