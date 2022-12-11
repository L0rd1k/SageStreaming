#pragma once 

#include <chrono>

class ElapsedTimer {
public:
    ElapsedTimer() :
    _beg(_clock_::now()) {}
    void start() {
        restart();
    }
    void restart() {
        _beg = _clock_::now();
    }
    const double elapsed() const {
        return std::chrono::duration_cast<_second_>(_clock_::now() - _beg).count();
    }
    const double elapsedMs() const {
        return elapsed() * 1000;
    }
    const double elapsedMcrs() const {
        return elapsed() * 100000;
    }
    bool expired(int ms) {
        return elapsedMs() > ms;
    }
private:
    using _clock_ = std::chrono::steady_clock;
    using _second_ = std::chrono::duration<double, std::ratio<1> >;
    std::chrono::time_point<_clock_> _beg;
};