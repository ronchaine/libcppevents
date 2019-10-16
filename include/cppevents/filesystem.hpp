#ifndef LIBCPPEVENTS_FILESYSTEM_HPP
#define LIBCPPEVENTS_FILESYSTEM_HPP

#include "events.hpp"

namespace cppevents
{
    class filesystem_event : public event
    {
        public:
            filesystem_event() : event(event_id) {}

            static inline event_typeid event_id = type_id_for<filesystem_event>();
    };
}

#endif
