#pragma once

#include <chrono>
#include <ctime>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

class Log {
public:
    Log() = default;

    virtual ~Log() {
        std::cout << std::endl;
    }

public:
    static std::string printTime() {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        std::string strTime = std::to_string(ltm->tm_hour) + ":" +
                              std::to_string(ltm->tm_min) + ":" +
                              std::to_string(ltm->tm_sec);
        return strTime;
    }
    
    template <class T>
    Log& operator<<(T value) {
        std::cout << value;
        return *this;
    }

    template <class T>
    Log& operator<<(std::vector<T> vec) {
        for (auto elem : vec) {
            std::cout << elem << " ";
        }
        return *this;
    }

    template <typename... Args>
    static void error(Args&&... args) {
        std::cout << "[" << printTime() << "]";
        ((std::cout << "\033[1;31;49m"
                    << " " << std::forward<Args>(args)),
         ...);
        std::cout << "\033[0m" << std::endl;
    }

    template <typename... Args>
    static void warning(Args&&... args) {
        std::cout << "[" << printTime() << "]";
        ((std::cout << "\033[1;35;49m"
                    << " " << std::forward<Args>(args)),
         ...);
        std::cout << "\033[0m" << std::endl;
    }

    template <typename... Args>
    static void trace(Args&&... args) {
        std::cout << "[" << printTime() << "]";
        ((std::cout << "\033[1;32;49m"
                    << " " << std::forward<Args>(args)),
         ...);
        std::cout << "\033[0m" << std::endl;
    }

    template <typename... Args>
    static void info(Args... args) {
        std::cout << "[" << printTime() << "]";
        ((std::cout << "\033[0;34;49m"
                    << " " << std::forward<Args>(args)),
         ...);
        std::cout << "\033[0m" << std::endl;
    }

    template <typename... Args>
    static void debug(Args&&... args) {
        ((std::cout << " " << std::forward<Args>(args)), ...);
        std::cout << std::endl;
    }

    template <typename... Args>
    static void critical(Args&&... args) {
        std::cout << "[" << printTime() << "]";
        ((std::cout << "\033[1;31;49m"
                    << " " << std::forward<Args>(args)),
         ...);
        std::cout << "\033[0m" << std::endl;
    }
};