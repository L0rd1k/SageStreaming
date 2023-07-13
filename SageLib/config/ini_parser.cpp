#include "ini_parser.h"

sage::IniParser& sage::IniParser::inst() {
    static IniParser inst;
    return inst;
}

bool sage::IniParser::setIniFile(std::string iniFilePath) {
    boost::property_tree::ini_parser::read_ini(iniFilePath, _ini_pt);
    if (_ini_pt.empty()) {
        Log::warning("Ini file path, not loaded or empty!");
        return true;
    }
    _iniFilePath = iniFilePath;
    return false;
}

std::string sage::IniParser::get(const std::string value, int8_t substanceId) {
    if(!value.empty()) {
        std::string group = (substanceId != -1) ? selectGroup(substanceId) : getMainGroup();
        try {
            auto result = _ini_pt.get<std::string>(group + "." + value);
            return result;
        } catch(std::exception& msg) {
            Log::critical(msg.what());
        }
    }
    return std::string();
}

std::string sage::IniParser::selectGroup(int8_t& substanceId) {
    return "Substance_" + std::to_string(substanceId);
}

std::string sage::IniParser::getMainGroup() {
    return "Main";
}