#ifndef LIBCPPEVENTS_WINDOW_HPP
#define LIBCPPEVENTS_WINDOW_HPP

#include "events.hpp"
#include "input.hpp"

namespace cppevents
{
    struct window_event
    {
    };

    struct window { using event_type = void; };
}

#endif
