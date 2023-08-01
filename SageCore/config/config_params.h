#pragma once 

#include "config/cfg_fields.h"


static const ConfigFields<uint64_t> CFG_SUBSTANCE_COUNT("substances_count");

static const ConfigFields<std::string> CFG_DEV_NAME("name");

static const ConfigFields<std::string> CFG_DEV_READER_TYPE("reader_type");
static const ConfigFields<std::string> CFG_DEV_DECODER_TYPE("decoder_type");
static const ConfigFields<std::string> CFG_DEV_ENCODER_TYPE("encoder_type");

static const ConfigFields<std::string> CFG_DEV_FFMPEG_CAPTURE_TYPE("ffmpegcap_type");
static const ConfigFields<std::string> CFG_DEV_OPENCV_CAPTURE_TYPE("cvcap_type");

static const ConfigFields<std::string> CFG_DEV_ENABLE_ON_BOOT("enable_on_boot");