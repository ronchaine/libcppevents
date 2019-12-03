#ifndef LIBCPPEVENTS_FILESYSTEM_HPP
#define LIBCPPEVENTS_FILESYSTEM_HPP

#include "events.hpp"

namespace cppevents
{
    class filesystem_event : public event
    {
        public:
            filesystem_event();
            ~filesystem_event();
            static inline event_typeid event_id = type_id_for<x>();

        private:
            class implementation;
            std::experimental::propagate_const<std::unique_ptr<implementation>> impl;
    };

    error_code add_filesystem_watch(const char* path, event_queue&);
    error_code remove_filesystem_watch(const char* path, event_queue&);
}

#endif
