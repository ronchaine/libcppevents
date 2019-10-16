#ifndef LIBCPPEVENT_EVENTS_HPP
#define LIBCPPEVENT_EVENTS_HPP

#include <atomic>
#include <type_traits>

#include "common.hpp"

namespace cppevents::detail
{
    inline std::atomic<uint64_t> event_typeid_counter = 0;
}

namespace cppevents
{
    // a base class for singular event
    class event
    {
        public:
            event(const event&) = delete;
            event_typeid event_type() const noexcept;

            virtual ~event();
    };

    template <typename T>
    event_typeid type_id_for()
    {
        static_assert(std::is_base_of<event, T>(), "Events need to derive from cppevents::event");
        static event_typeid evid = detail::event_typeid_counter++;
        return evid;
    }

    // event queue is what we report our events to
    class event_queue
    {
        public:
            template <typename T>
            void on_event(std::function<void(event)>);

        private:
            std::vector<std::function<void(event)>> callbacks;
    };

    error_code send_event(event_queue&, event);
    error_code send_event(event);
}

namespace cppevents::detail
{
    inline event_queue default_queue;
}

#endif
