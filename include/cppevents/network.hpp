#ifndef LIBCPPEVENTS_NETWORK_HPP
#define LIBCPPEVENTS_NETWORK_HPP

#include "events.hpp"

#include <string>

namespace cppevents
{
    struct network_event
    {
        // POSIX
        using native_socket_type = int;
        // winsock
        // using native_socket_type = SOCKET;

        enum subtype {
            new_connection,
            socket_ready
        };

        subtype type;

        std::string peer_address;

        native_socket_type sock_handle = -1;
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
