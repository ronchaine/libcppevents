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

            static filesystem_event translate(std::vector<uint8_t>& data);

        private:
            class implementation;
            std::experimental::propagate_const<std::unique_ptr<implementation>> impl;
    };
    
    /**
     * Add filesystem watch
     *
     * \param   path_name   filesystem path to monitor
     * \param   queue       event queue to report to
     *
     * \returns error code depicting either success or reason for failure
     */
    error_code watch_path(const std::string& path_name, event_queue& queue = default_queue);
}

#endif
