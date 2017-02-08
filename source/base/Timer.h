#ifndef BASE_TIMER_H
#define BASE_TIMER_H

#include <chrono>

#include "common/Object.h"

using namespace std::chrono;

namespace te
{
    class Timer: public Object
    {
    private:
        using TimePoint = system_clock::time_point;
        using TimeDuration = duration<float>;

    public:
        Timer();
        ~Timer();

        void setEnabled(bool enabled);
        void reset();
        float getElapsedTime();
    private:
        TimePoint getCurrentTime();

    private:
        bool _enabled;
        TimePoint _start;
        float _elapsed;
    };
}


#endif // BASE_TIMER_H