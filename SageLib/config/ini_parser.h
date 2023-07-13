#pragma once

#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <mutex>

#include "utils/log.h"
#include "cfg_fields.h"

namespace sage {

class IniParser {
public:
    IniParser() = default;
    static IniParser& inst();
    bool setIniFile(std::string iniFilePath);
    std::string get(const std::string value = std::string(), int8_t substanceId = -1);

    template <typename T>
    T get(const ConfigFields<T>& value, int8_t substanceId = -1) {
        if (!value.name().empty()) {
            std::string group = (substanceId != -1) ? selectGroup(substanceId) : getMainGroup();
            try {
                auto result = _ini_pt.get<T>(group + "." + value.name());
                return result;
            } catch (std::exception& msg) {
                Log::critical(msg.what());
            }
        }
        return T();
    }

private:
    std::string selectGroup(int8_t& substanceId);
    std::string getMainGroup();
    std::string _iniFilePath;
    boost::property_tree::ptree _ini_pt;
};

}  // namespace sage