#ifndef LIBCPPEVENTS_WINDOW_HPP
#define LIBCPPEVENTS_WINDOW_HPP

#include "events.hpp"
#include "input.hpp"

namespace cppevents
{
    struct window_event
    {
    };

    // can send both window and input events
    struct window { using event_type = void; };
}

#endif
