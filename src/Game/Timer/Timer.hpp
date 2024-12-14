/*
    Authors:
    >> Caroline Boilly @Aeliondw
    >> Nathan Tirolf @dragusheen
    >> Daniil Stepanov @dan13615

    („• ֊ •„)❤  <  Have a good day !
    --U-----U------------------------
*/

#ifndef TIMER_HPP
    #define TIMER_HPP

#include "RType.hpp"

class Timer
{
private:
    std::chrono::steady_clock::time_point _startTime;
    std::chrono::milliseconds _interval;

public:
    Timer(int intervalMs);
    ~Timer() = default;

    void reset();
    bool hasElapsed() const;
    int timeLeft() const;
};

#endif // TIMER_HPP