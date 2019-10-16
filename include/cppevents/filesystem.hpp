#ifndef LIBCPPEVENTS_FILESYSTEM_HPP
#define LIBCPPEVENTS_FILESYSTEM_HPP

#include "events.hpp"

namespace cppevents
{
    class filesystem_event : public event
    {
        public:
            std::string file;

        private:
            event_typeid;
    };
}

#endif
