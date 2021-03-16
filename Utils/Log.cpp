#include "Log.h"

Log::Log() {
}

inline constexpr std::uint32_t hashing_fnv1a(const char* str, std::uint32_t hash = 2166136261UL) {
    if(hashing_fnv1a(str + 1, (hash ^ *str) * 16777619ULL)) {
        return *str;  
    } else {
        return hash;
    }
}

uint64_t constexpr mix(char m, uint64_t s) {
  return ((s << 7) + ~(s >> 3)) + ~m;
}
 
uint64_t constexpr hash(const char * m) {
  return (*m) ? mix(*m, hash(m+1)) : 0;
}

Log::Log(ColorSpaceCode code) {
    switch(code) {
        case ColorSpaceCode::RED:
            color_code = ColorSpaceCode::RED;
            break;
        case ColorSpaceCode::GREEN:
            color_code = ColorSpaceCode::GREEN;
            break; 
        case ColorSpaceCode::BLUE:
            color_code = ColorSpaceCode::BLUE;
            break;
        case ColorSpaceCode::YELLOW:
            color_code = ColorSpaceCode::YELLOW;
            break;
        case ColorSpaceCode::BLACK:
            color_code = ColorSpaceCode::BLACK;
            break;
        case ColorSpaceCode::MAGENTA:
            color_code = ColorSpaceCode::MAGENTA;
            break;
        case ColorSpaceCode::CYAN:
            color_code = ColorSpaceCode::CYAN;
            break;
        case ColorSpaceCode::WHITE:
            color_code = ColorSpaceCode::WHITE;
            break;
        default:
            break;
    }
}

void Log::printTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::string strTime = std::ctime(&t);
    std::cout << strTime.substr(0, strTime.length() - 1) << ": ";
}

Log::Log(std::string color) {
    switch(hash(color.c_str())){
        case hash("red"):
            color_code = ColorSpaceCode::RED;
            break;
    }
}



Log::~Log() {
}