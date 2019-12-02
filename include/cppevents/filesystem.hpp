#ifndef LIBCPPEVENTS_FILESYSTEM_HPP
#define LIBCPPEVENTS_FILESYSTEM_HPP

#include "events.hpp"

namespace cppevents
{
    class filesystem_event : public event
    {
        CPPEV_DECLARE_EVENT(filesystem_event);
    };

    error_code add_filesystem_watch(const char* path, event_queue&);
    error_code remove_filesystem_watch(const char* path, event_queue&);
}

#endif
