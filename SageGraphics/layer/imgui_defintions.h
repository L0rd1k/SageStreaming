#pragma once

#include <string>

#include "utils/elapsed_timer.h"

const std::string settingsWinName = "Settings";
const std::string loggingWinName = "Logger";
const std::string managerWinName = "Manager";

struct ImGuiCameraSettings {
    ImGuiCameraSettings()
        : isAspectRatio_(true),
          isCamActive_(false) {
    }

    bool* getAspectRatio() {
        return &isAspectRatio_;
    }

    bool* getCameraActivity() {
        return &isCamActive_;
    }

    struct PlotFps {
        PlotFps()
            : fpsArr{},
              offset(-1) {
        }
        float fpsArr[25];
        int64_t offset;
        ElapsedTimer timer;
    };

    PlotFps* plot() {
        return &plotFps_;
    }

private:
    bool isAspectRatio_;
    bool isCamActive_;
    PlotFps plotFps_;
};

const double settingsWinCap = 0.2f;     //< Settings window width
const double camSettingsWinCap = 0.6f;  //< Camera settings window width
const double loggingWinCap = 0.2f;      //< Logging window width
const double managerWinCap = 1.0f;      //< Manager window width

static const char* comboBoxReaderType =
    "FFmpeg\0"
    "OpenCV\0";
static const char* comboBoxCapTypeFFmpeg =
    "Tcp\0"
    "Udp\0"
    "V4L\0"
    "Video\0";
static const char* comboBoxCapTypeOpenCV =
    "Gstreamer\0"
    "FFmpeg\0"
    "V4L\0"
    "Any\0";
static const char* comboBoxTextureSize =
    "2048x1536\0"
    "1024x768\0"
    "800x600\0"
    "640x480\0"
    "320x240\0";

static const char* comboBoxDecoderType =
    "FFmpeg\0";