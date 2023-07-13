#pragma once

#include <cstdint>
#include <unordered_map>

template <typename T>
class ConfigFields {
public:
    ConfigFields(std::string fieldName)
        : fname(fieldName) {
    };
    ~ConfigFields() = default;
    const std::string name() const  {
        return fname;
    }
private:
    const std::string fname;
};
