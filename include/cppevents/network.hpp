#ifndef LIBCPPEVENTS_NETWORK_HPP
#define LIBCPPEVENTS_NETWORK_HPP

#include "events.hpp"

#include <string>

namespace cppevents
{
    struct network_event
    {
        #if defined (_WIN32)
        using native_socket_type = SOCKET;
        #else
        // POSIX
        using native_socket_type = int;
        #endif

        enum subtype {
            new_connection,
            socket_ready,
            connection_closed
        };

        subtype type;

        std::string peer_address;
        uint16_t    peer_port = 0;

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
