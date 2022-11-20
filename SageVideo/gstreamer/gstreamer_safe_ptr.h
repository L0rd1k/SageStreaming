#pragma once

#include <gst/gst.h>
#include <gst/pbutils/encoding-profile.h>

#include <algorithm>
#include <assert.h>

namespace sage {

namespace gstrmr {

#if defined __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"
#endif

template <typename T>
static inline void GSafePtr_addref(T* ptr) {
    if (ptr) g_object_ref_sink(ptr);
}

template <typename T>
static inline void GSafePtr_release(T** pPtr);

template <>
inline void GSafePtr_release<GError>(GError** pPtr) {
    g_clear_error(pPtr);
}
template <>
inline void GSafePtr_release<GstElement>(GstElement** pPtr) {
    if (pPtr) {
        gst_object_unref(G_OBJECT(*pPtr));
        *pPtr = NULL;
    }
}
template <>
inline void GSafePtr_release<GstElementFactory>(GstElementFactory** pPtr) {
    if (pPtr) {
        gst_object_unref(G_OBJECT(*pPtr));
        *pPtr = NULL;
    }
}
template <>
inline void GSafePtr_release<GstPad>(GstPad** pPtr) {
    if (pPtr) {
        gst_object_unref(G_OBJECT(*pPtr));
        *pPtr = NULL;
    }
}
template <>
inline void GSafePtr_release<GstCaps>(GstCaps** pPtr) {
    if (pPtr) {
        gst_caps_unref(*pPtr);
        *pPtr = NULL;
    }
}
template <>
inline void GSafePtr_release<GstBuffer>(GstBuffer** pPtr) {
    if (pPtr) {
        gst_buffer_unref(*pPtr);
        *pPtr = NULL;
    }
}
template <>
inline void GSafePtr_release<GstSample>(GstSample** pPtr) {
    if (pPtr) {
        gst_sample_unref(*pPtr);
        *pPtr = NULL;
    }
}
template <>
inline void GSafePtr_release<GstBus>(GstBus** pPtr) {
    if (pPtr) {
        gst_object_unref(G_OBJECT(*pPtr));
        *pPtr = NULL;
    }
}
template <>
inline void GSafePtr_release<GstMessage>(GstMessage** pPtr) {
    if (pPtr) {
        gst_message_unref(*pPtr);
        *pPtr = NULL;
    }
}
template <>
inline void GSafePtr_release<GMainLoop>(GMainLoop** pPtr) {
    if (pPtr) {
        g_main_loop_unref(*pPtr);
        *pPtr = NULL;
    }
}

template <>
inline void GSafePtr_release<GstEncodingVideoProfile>(
    GstEncodingVideoProfile** pPtr) {
    if (pPtr) {
        gst_encoding_profile_unref(*pPtr);
        *pPtr = NULL;
    }
}
template <>
inline void GSafePtr_release<GstEncodingContainerProfile>(
    GstEncodingContainerProfile** pPtr) {
    if (pPtr) {
        gst_object_unref(G_OBJECT(*pPtr));
        *pPtr = NULL;
    }
}

template <>
inline void GSafePtr_addref<char>(
    char* pPtr);  // declaration only. not defined. should not be used
template <>
inline void GSafePtr_release<char>(char** pPtr) {
    if (pPtr) {
        g_free(*pPtr);
        *pPtr = NULL;
    }
}

#if defined __clang__
#pragma clang diagnostic pop
#endif

template <typename T>
class GSafePtr {
   protected:
    T* ptr;

   public:
    inline GSafePtr() noexcept : ptr(NULL) {}
    inline ~GSafePtr() noexcept { release(); }
    inline void release() noexcept {
        if (ptr) GSafePtr_release<T>(&ptr);
    }

    inline operator T*() noexcept { return ptr; }
    inline operator/*const*/ T*() const noexcept {
        return (T*)ptr;
    }  // there is no const correctness in Gst C API

    T* get() {
        assert(ptr);
        return ptr;
    }
    /*const*/ T* get() const {
        assert(ptr);
        return (T*)ptr;
    }  // there is no const correctness in Gst C API

    const T* operator->() const {
        assert(ptr);
        return ptr;
    }
    inline operator bool() const noexcept { return ptr != NULL; }
    inline bool operator!() const noexcept { return ptr == NULL; }

    T** getRef() {
        assert(ptr == NULL);
        return &ptr;
    }

    inline GSafePtr& reset(T* p) noexcept  // pass result of functions with
                                           // "transfer floating" ownership
    {
        // printf("reset: %s:%d: %p\n", CV__TRACE_FUNCTION, __LINE__, p);
        release();
        if (p) {
            GSafePtr_addref<T>(p);
            ptr = p;
        }
        return *this;
    }

    inline GSafePtr& attach(T* p) noexcept  // pass result of functions with
                                            // "transfer full" ownership
    {
        // printf("attach: %s:%d: %p\n", CV__TRACE_FUNCTION, __LINE__, p);
        release();
        ptr = p;
        return *this;
    }
    inline T* detach() noexcept {
        T* p = ptr;
        ptr = NULL;
        return p;
    }

    inline void swap(GSafePtr& o) noexcept { std::swap(ptr, o.ptr); }

   private:
    GSafePtr(const GSafePtr&);      // = disabled
    GSafePtr& operator=(const T*);  // = disabled
};

}  // namespace gstrmr

}  // namespace sage
