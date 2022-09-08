#pragma once

#include <mutex>

#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "utils/log.h"

namespace sage {

class IniParser {
public:
    IniParser() = default;
    static IniParser& inst();
    bool setIniFile(std::string iniFilePath);
    std::string get(const std::string value, uint8_t substanceId);
private:
    std::string selectGroup(uint8_t &substanceId);
    std::string _iniFilePath;
    boost::property_tree::ptree _ini_pt;
};

}  // namespace sage