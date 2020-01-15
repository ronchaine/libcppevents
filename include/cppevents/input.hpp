#ifndef LIBCPPEVENTS_INPUT_HPP
#define LIBCPPEVENTS_INPUT_HPP

#include "events.hpp"

namespace cppevents
{
    struct input_event
    {
    };

    struct input { using event_type = input_event; };
}

#endif
