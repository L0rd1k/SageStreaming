#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <vector>
#include <chrono>
#include <ctime>


class Log {
public:
    Log() = default;

    virtual ~Log() {
        std::cout << std::endl;
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
        std::cout << value;
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