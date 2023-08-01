#include "substance_config.h"

sage::SubstanceConfig::SubstanceConfig(short id) : _id(id) {}

const bool sage::SubstanceConfig::init() {
    _camType = toCamType(sage::IniParser::inst().get(CFG_DEV_READER_TYPE, _id));
    _encType = toEncType(sage::IniParser::inst().get(CFG_DEV_ENCODER_TYPE, _id));
    _decType = toDecType(sage::IniParser::inst().get(CFG_DEV_DECODER_TYPE, _id));
    _ffmpegType = toFFmpegCapType(sage::IniParser::inst().get(CFG_DEV_FFMPEG_CAPTURE_TYPE, _id));
    _opencvType = toOpenCVCapType(sage::IniParser::inst().get(CFG_DEV_OPENCV_CAPTURE_TYPE, _id));
    _isEnableOnBoot = std::stoi(sage::IniParser::inst().get(CFG_DEV_ENABLE_ON_BOOT, _id));

    _url = sage::IniParser::inst().get(CFG_DEV_NAME, _id);
    if ((_camType == sage::CamTypes::Undefined) || (_decType == sage::DecTypes::Undefined)) {
        return false;
    }
    return true;
}

void sage::SubstanceConfig::setCamReaderType(const sage::CamTypes type) {
    _camType = type;
}

void sage::SubstanceConfig::setCamDecoderType(const sage::DecTypes type) {
    _decType = type;
}

void sage::SubstanceConfig::setCamEncoderType(const sage::EncTypes type) {
    _encType = type;
}

void sage::SubstanceConfig::setCamUrl(const std::string url) {
    _url = url;
}

void sage::SubstanceConfig::setFFmpegCaptureType(const sage::FFmpegType type) {
    _ffmpegType = type;
}

void sage::SubstanceConfig::setOpenCVCaptureType(const sage::OpencvType type) {
    _opencvType = type;
}

void sage::SubstanceConfig::setId(short id) {
    _id = id;
}

void sage::SubstanceConfig::setSubstEnabled(bool val) {
    _isEnableOnBoot = val;
}

const sage::CamTypes sage::SubstanceConfig::getCamReaderType() {
    return _camType;
}

const sage::DecTypes sage::SubstanceConfig::getCamDecoderType() {
    return _decType;
}

const sage::EncTypes sage::SubstanceConfig::getCamEncoderType() {
    return _encType;
}

const sage::FFmpegType sage::SubstanceConfig::getFFmpegCaptureType() {
    return _ffmpegType;
}

const sage::OpencvType sage::SubstanceConfig::getOpenCVCaptureType() {
    return _opencvType;
}

const std::string sage::SubstanceConfig::getCamUrl() {
    return _url;
}

const short sage::SubstanceConfig::getId() {
    return _id;
}

const bool sage::SubstanceConfig::isSubstEnabled() {
    return _isEnableOnBoot;
}

void sage::SubstanceConfig::saveToConfigFile(const sage::SubstanceState& camState) {
    sage::IniParser::inst().set(CFG_SUBSTANCE_COUNT, sage::IniParser::inst().get(CFG_SUBSTANCE_COUNT) + 1);

    sage::IniParser::inst().set(CFG_DEV_NAME, camState.url, _id);
    sage::IniParser::inst().set(CFG_DEV_READER_TYPE, toString(camState.camType), _id);
    sage::IniParser::inst().set(CFG_DEV_DECODER_TYPE, toString(camState.decType), _id);
    sage::IniParser::inst().set(CFG_DEV_ENCODER_TYPE, toString(camState.encType), _id);
    sage::IniParser::inst().set(CFG_DEV_FFMPEG_CAPTURE_TYPE, toString(camState.capTypeFFmpeg), _id);
    sage::IniParser::inst().set(CFG_DEV_OPENCV_CAPTURE_TYPE, toString(camState.capTypeOpencv), _id);
    sage::IniParser::inst().set(CFG_DEV_ENABLE_ON_BOOT, std::to_string(camState.isSubstEnabled), _id);
}
