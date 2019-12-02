#ifndef LIBCPPEVENT_EVENTS_HPP
#define LIBCPPEVENT_EVENTS_HPP

#include <atomic>
#include <type_traits>

#include <vector>
#include <functional>

#include <experimental/propagate_const>
#include <memory>

#include "common.hpp"

#define CPPEV_DECLARE_EVENT(x) public:    \
                                  x();    \
                                  ~x();   \
                                  static inline event_typeid event_id = type_id_for<x>(); \
                               private:   \
                                  class implementation; \
                                  std::experimental::propagate_const<std::unique_ptr<implementation>> impl;

namespace cppevents::detail
{
    inline std::atomic<event_typeid> event_typeid_counter = 0;
}

namespace cppevents
{
    class event;
    class event_queue;

    using event_callback_type = std::function<void(event&)>;

    // tag for enabling all events
    struct any_event{};

    // a base class for singular event
    class event
    {
        public:
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
        static event_typeid evid = 1ul << detail::event_typeid_counter++;
        return evid;
    }

    // event queue is what we report our events to
    class event_queue
    {
        public:
            template <typename T, typename... Others>
            void on_event(event_callback_type);

            void wait() {}
            void poll();

        private:
//            event_typeid listening_event_types = NO_EVENTS;

            std::unordered_map<event_typeid, event_callback_type> callbacks;
    };

    error_code send_event(event_queue&, event);
    error_code send_event(event);

    inline event_queue default_queue;
}

namespace cppevents
{
    template <typename T, typename... Others>
    void event_queue::on_event(event_callback_type)
    {
    }
}

#endif
