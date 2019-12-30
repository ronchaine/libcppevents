#ifndef LIBCPPEVENTS_NETWORK_HPP
#define LIBCPPEVENTS_NETWORK_HPP

#include "events.hpp"

#include <string>

namespace cppevents
{
    struct network_event
    {
        /*
        static const inline event_typeid event_id = get_event_id_for<network_event>();
        network_event() : event(event_id) {}
        */
    };

    struct socket_listener
    {
        using event_type = network_event;
    };

    struct socket_io_event
    {
        using event_type = network_event;
    };
}

#endif
