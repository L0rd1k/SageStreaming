#pragma once 

#include "definitions/local_definitions.h"
#include "config/ini_parser.h"

namespace sage {

class SubstanceConfig {
public:
    SubstanceConfig(short id);
    const bool init();

    void setCamReaderType(const sage::CamTypes type);
    void setCamDecoderType(const sage::DecTypes type);
    void setCamEncoderType(const sage::EncTypes type);
    void setFFmpegCaptureType(const sage::FFmpegType type);
    void setOpenCVCaptureType(const sage::OpencvType type);
    void setCamUrl(const std::string url);

    const sage::CamTypes getCamReaderType();
    const sage::DecTypes getCamDecoderType();
    const sage::EncTypes getCamEncoderType();
    const sage::FFmpegType getFFmpegCaptureType();
    const sage::OpencvType getOpenCVCaptureType();
    const std::string getCamUrl();
    const short getId();
private:
    short _id;
    sage::CamTypes _camType;        //> Camera Reader Type
    sage::EncTypes _encType;        //> Camera Encoder Type
    sage::DecTypes _decType;        //> Camera Decoder Type
    sage::FFmpegType _ffmpegType;   //> FFmpeg input Type
    sage::OpencvType _opencvType;   //> Opencv input Type
    std::string _url;

};

}