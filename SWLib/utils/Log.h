#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <vector>
#include <chrono>
#include <ctime>


enum class Status {
    DEFAULT = 0,
    WARNING = 1,
    ERROR = 2,
    SUCCES = 3,
};


class Log {
public:
    Log() = default;
    Log(std::string color) {
    }

    virtual ~Log() {
    }

public:
    static void printTime() {
        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        std::string strTime = std::ctime(&t);
        std::cout << strTime.substr(0, strTime.length() - 1) << ": ";        
    }
    template<class T>
    Log& operator<<(T value) {
        // std::cout << "\033[" << (int)color_code << "m" << value;
        // std::cout << "\033[" << 39 << "m";
        std::cout << value << std::endl;
        return *this;
    }

    template<class T>
    Log& operator<<(std::vector<T> vec) {
        for(auto elem : vec) {
            std::cout << elem << " ";
        }
        return *this;
    }
    
};

#endif  /* LOG_H */

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
