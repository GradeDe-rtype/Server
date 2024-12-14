/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Timer class
** Caroline Boilly @Aeliondw
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