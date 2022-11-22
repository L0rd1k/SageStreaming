#pragma once

#include "gstreamer_safe_ptr.h"
#include "utils/log.h"

#include <thread>

namespace gstrmr {

/*!
 * \brief The GstInitializer class
 * Initializes gstreamer once in the whole process
 */
class GstInitializer {
public:
    static GstInitializer& init();
private:
    bool isFailed;
    bool call_deinit;

    GstInitializer();
    ~GstInitializer();
};

}  // namespace gstrmr