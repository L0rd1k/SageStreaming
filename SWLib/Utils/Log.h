#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <vector>
#include <chrono>

enum class COLOR {
        RED      = 31,
        GREEN    = 32,
        BLUE     = 34,
        DEFAULT  = 39,
        YELLOW   = 33,
        BLACK    = 30,
        MAGENTA  = 35,
        CYAN     = 36,
        WHITE    = 37
};

inline constexpr std::uint32_t hashing_fnv1a(const char* str, std::uint32_t hash);

class Log {
public:
    Log();
    Log(std::string);
    Log(COLOR);
    virtual ~Log();
    COLOR color_code;
public:
    static void printTime();
    template<class T>
    Log& operator<<(T value) {
        std::cout << "\033[" << (int)color_code << "m" << value;
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