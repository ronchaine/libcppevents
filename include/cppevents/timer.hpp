#ifndef LIBCPPEVENTS_TIMER_HPP
#define LIBCPPEVENTS_TIMER_HPP

#include "events.hpp"

namespace cppevents
{
    class system_timer_event
    {
        public:
            system_timer_event() : event(event_id) {}

            static inline event_typeid event_id = type_id_for<system_timer_event>();
    };

    class timer
    {
    };
}

#endif
