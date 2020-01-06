#ifndef LIBCPPEVENTS_SIGNAL_HPP
#define LIBCPPEVENTS_SIGNAL_HPP

#include "events.hpp"

namespace cppevents
{
    struct signal_event
    {
        int signal_no   = -1;
        int sender_pid  = -1;
        int sender_uid  = -1;
        int trap_no     = -1;
        int status      = -1;
    };

    struct signal { using event_type = signal_event; };
}

#endif
