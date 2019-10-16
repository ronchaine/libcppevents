#ifndef LIBCPPEVENT_EVENTS_HPP
#define LIBCPPEVENT_EVENTS_HPP

#include <atomic>
#include <type_traits>

#include <vector>
#include <functional>

#include "common.hpp"

namespace cppevents::detail
{
    inline std::atomic<event_typeid> event_typeid_counter = 1;
}

namespace cppevents
{
    // a base class for singular event
    class event
    {
        public:
            virtual ~event() {}

            event(const event&) = delete;

            event_typeid type() const noexcept { return id; }

        protected:
            explicit event(event_typeid id) noexcept : id(id) {}

        private:
            event_typeid id;
    };

    template <typename T>
    event_typeid type_id_for()
    {
        static_assert(std::is_base_of<event, typename std::decay<T>::type>(), "Events need to derive from cppevents::event");
        static event_typeid evid = detail::event_typeid_counter;
        detail::event_typeid_counter = detail::event_typeid_counter << 1;
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

    class dummy_event : public event
    {
        public:
            dummy_event() : event(event_id) {}

            static inline event_typeid event_id = type_id_for<dummy_event>();
    };
}

namespace cppevents::detail
{
    inline event_queue default_queue;
}

#endif
