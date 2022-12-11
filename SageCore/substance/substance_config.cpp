#include "substance_config.h"

sage::SubstanceConfig::SubstanceConfig(short id) : _id(id) {}

const bool sage::SubstanceConfig::init() {
    _camType = toCamType(sage::IniParser::inst().get("reader_type", _id));
    _encType = toEncType(sage::IniParser::inst().get("encoder_type", _id));
    _decType = toDecType(sage::IniParser::inst().get("decoder_type", _id));
    _ffmpegType = toFFmpegCapType(sage::IniParser::inst().get("ffmpegcap_type", _id));
    _opencvType = toOpenCVCapType(sage::IniParser::inst().get("cvcap_type", _id));
    _url = sage::IniParser::inst().get("name", _id);
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
