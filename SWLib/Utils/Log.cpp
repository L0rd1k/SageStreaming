#include "Log.h"


inline constexpr std::uint32_t hashing_fnv1a(const char* str, std::uint32_t hash = 2166136261UL) {
    if(hashing_fnv1a(str + 1, (hash ^ *str) * 16777619ULL)) {
        return *str;  
    } 
    return hash;
}

uint64_t constexpr mix(char m, uint64_t s) {
  return ((s << 7) + ~(s >> 3)) + ~m;
}
 
uint64_t constexpr hash(const char * m) {
  return (*m) ? mix(*m, hash(m+1)) : 0;
}

Log::Log(COLOR code) {
    switch(code) {
        case COLOR::RED:
            color_code = COLOR::RED;
            break;
        case COLOR::GREEN:
            color_code = COLOR::GREEN;
            break; 
        case COLOR::BLUE:
            color_code = COLOR::BLUE;
            break;
        case COLOR::YELLOW:
            color_code = COLOR::YELLOW;
            break;
        case COLOR::BLACK:
            color_code = COLOR::BLACK;
            break;
        case COLOR::MAGENTA:
            color_code = COLOR::MAGENTA;
            break;
        case COLOR::CYAN:
            color_code = COLOR::CYAN;
            break;
        case COLOR::WHITE:
            color_code = COLOR::WHITE;
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
            color_code = COLOR::RED;
            break;
    }
}



Log::~Log() {
}