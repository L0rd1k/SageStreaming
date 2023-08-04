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


    template <typename T>
    void set(const ConfigFields<T>& fieldName, const T value, int8_t substanceId = -1) {
        if(!fieldName.name().empty()) {
            std::string group = (substanceId != -1) ? selectGroup(substanceId) : getMainGroup();
            try {
                std::cout << group << " " <<  fieldName.name() << " " << value << std::endl;
                _ini_pt.put(group + "." + fieldName.name(), value);
                boost::property_tree::ini_parser::write_ini(_iniFilePath, _ini_pt);
            } catch(std::exception& msg) {
                Log::critical(msg.what());
            }
        }
    }

    void remove(int8_t substanceId, size_t nodeCount) {
        std::cout << "INI ERASE: " << (int)substanceId << " " << nodeCount << std::endl;

        if(substanceId >= nodeCount) {
            substanceId-=counterPadding;
        }

        _ini_pt.erase(selectGroup(substanceId));

        std::cout << "INI ERASE 2: " << (int)substanceId << " " << nodeCount << std::endl;

        int8_t nextIdx = substanceId + 1;
        for(int8_t indx = substanceId; indx < nodeCount; indx++) {
            std::cout << (int)indx << " " << (int)nextIdx << std::endl;
            _ini_pt.add_child(selectGroup(indx), _ini_pt.get_child(selectGroup(nextIdx)));
            _ini_pt.erase(selectGroup(nextIdx)); 
            nextIdx++;
        } 
        counterPadding++;
        boost::property_tree::ini_parser::write_ini(_iniFilePath, _ini_pt);
    }

private:
    std::string selectGroup(int8_t& substanceId);
    std::string getMainGroup();
    std::string _iniFilePath;
    boost::property_tree::ptree _ini_pt;

    int counterPadding = 0;
};

}  // namespace sage