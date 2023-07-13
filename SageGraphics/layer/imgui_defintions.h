#pragma once

#include <string>

#include "utils/elapsed_timer.h"

const std::string settingsWinName = "Settings";
const std::string loggingWinName = "Logger";
const std::string managerWinName = "Manager";

struct PlottingSubstInfo {
    float pltFpsValues[25];
    int pltValOffset;
    ElapsedTimer pltTimerFps;

    PlottingSubstInfo()
        : pltFpsValues{},
          pltValOffset(0) {
    }
};

struct ImGuiCameraSettings {
    ImGuiCameraSettings()
        : isAspectRatio_(false) {
    }

    bool* getAspectRatio() {
        return &isAspectRatio_;
    }

private:
    bool isAspectRatio_;
};

const double settingsWinCap = 0.2f;     //< Settings window width
const double camSettingsWinCap = 0.6f;  //< Camera settings window width
const double loggingWinCap = 0.2f;      //< Logging window width
const double managerWinCap = 0.3f;      //< Manager window width

static const char* combobox_readerTypes =
    "FFmpeg\0"
    "OpenCV\0";
static const char* combobox_ffmpegCapTypes =
    "Tcp\0"
    "Udp\0"
    "V4L\0"
    "Video\0";
static const char* combobox_opencvCapTypes =
    "Gstreamer\0"
    "FFmpeg\0"
    "V4L\0"
    "Any\0";
static const char* combobox_textureSize =
    "2048x1536\0"
    "1024x768\0"
    "800x600\0"
    "640x480\0"
    "320x240\0";

static const char* combobox_decoderType =
    "FFmpeg\0";