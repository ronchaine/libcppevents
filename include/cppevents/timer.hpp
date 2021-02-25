#ifndef LIBCPPEVENTS_TIMER_HPP
#define LIBCPPEVENTS_TIMER_HPP

#include "event_queue.hpp"

namespace cppevents::event
{
    struct timer {
        int timer_id;
    };
}

namespace cppevents::detail {
    template<class T>
    struct is_duration : std::false_type {};

    template<class Rep, class Period>
    struct is_duration<std::chrono::duration<Rep, Period>> : std::true_type {};
}

namespace cppevents
{
    struct timer
    {
        template <typename T> requires detail::is_duration<T>::value
        constexpr timer(int id, T duration, int reps = -1) noexcept : id{id}, repeats{reps}
        {
            auto nanosecs = std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
            auto secs = std::chrono::duration_cast<std::chrono::seconds>(duration);

            seconds = (secs - nanosecs).count();
            nanoseconds = nanosecs.count();
        }

        uint64_t seconds;
        uint64_t nanoseconds;

        int id;
        int repeats;
    };
}

#endif
