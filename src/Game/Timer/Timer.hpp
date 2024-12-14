/*
    Authors:
    >> Caroline Boilly @Aeliondw
    >> Nathan Tirolf @dragusheen
    >> Daniil Stepanov @dan13615

    („• ֊ •„)❤  <  Have a good day !
    --U-----U------------------------
*/

#ifndef RTYPE_GAME_TIMER_HPP_
#define RTYPE_GAME_TIMER_HPP_

#include <chrono>
#include <algorithm>

namespace RType
{
    namespace Game
    {
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
    } // namespace Game
} // namespace RType

#endif // RTYPE_GAME_TIMER_HPP_