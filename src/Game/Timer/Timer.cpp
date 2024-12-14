/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Timer class
** Caroline Boilly @Aeliondw
*/

#include "Timer.hpp"


namespace Server
{
    Timer::Timer(int intervalMs) : _interval(intervalMs)
    {
        _startTime = std::chrono::steady_clock::now();
    }

    void Timer::reset()
    {
        _startTime = std::chrono::steady_clock::now();
    }

    bool Timer::hasElapsed() const
    {
        auto now = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(now - _startTime) >= _interval;
    }

    int Timer::timeLeft() const
    {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - _startTime);
        return std::max(0, static_cast<int>(_interval.count() - elapsed.count()));
    }
}