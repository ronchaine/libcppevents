/*!
 *  \file       network.hpp
 *  \brief      network event types for libcppevents
 *  \author     Jari Ronkainen
 *  \version    0.6
 *
 *  \todo Windows support
 */
#ifndef LIBCPPEVENTS_NETWORK_HPP
#define LIBCPPEVENTS_NETWORK_HPP

#include "event_queue.hpp"

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

        enum subtype : unsigned int {
            new_connection,
            socket_ready,
            connection_closed
        };

        subtype type;

        std::string peer_address;
        uint16_t    peer_port = 0;

        native_socket_type sock_handle = -1;
    };

    struct socket_listener { using event_type = network_event; };
    struct socket_io_event { using event_type = network_event; };
}

#endif
/*
    Copyright (c) 2020 Jari Ronkainen

    This software is provided 'as-is', without any express or implied warranty.
    In no event will the authors be held liable for any damages arising from the
    use of this software.

    Permission is granted to anyone to use this software for any purpose, including
    commercial applications, and to alter it and redistribute it freely, subject to
    the following restrictions:

    1. The origin of this software must not be misrepresented; you must not claim
       that you wrote the original software. If you use this software in a product,
       an acknowledgment in the product documentation would be appreciated but is
       not required.

    2. Altered source versions must be plainly marked as such, and must not be
       misrepresented as being the original software.

    3. This notice may not be removed or altered from any source distribution.
*/
