#ifndef LIBCPPEVENT_EVENTS_HPP
#define LIBCPPEVENT_EVENTS_HPP

#include <atomic>
#include <type_traits>

#include <functional>
#include <chrono>
#include <array>

#include <experimental/propagate_const>
#include <memory>

#include "common.hpp"
#include "event.hpp"

namespace cppevents
{
    class event_queue;

    using event_callback_type = std::function<void(event&)>;

    // tag for enabling all events
    struct any_event{};

    // file descriptor for POSIX
    using native_source_type = int;
    using translator_type = event(*)(native_source_type);
    using destructor_type = void(*)(native_source_type);

    // event queue is what we report our events to
    class event_queue
    {
        public:
            event_queue();
            ~event_queue();

            template <typename T, typename... Others>
            void on_event(event_callback_type callback) { on_event(get_event_id_for<T>() | get_event_id_for<Others...>(), callback); }
            void on_event(event_typeid, event_callback_type);

            void wait(std::chrono::milliseconds timeout = std::chrono::milliseconds(-1));
            void poll();

            // This is needed only for adding new events
            error_code add_native_source(native_source_type, translator_type, destructor_type = nullptr);
            void remove_native_source(native_source_type);

            template <typename EventType>
            error_code send_event(EventType ev) { return send_event(get_event_id_for<EventType>(), ev); }
            error_code send_event(event_typeid, event);

        private:
            class implementation;
            std::experimental::propagate_const<std::unique_ptr<implementation>> impl;
        };

    inline event_queue default_queue;

    template <typename Source, typename T>
    error_code add_source(T&, event_queue& = default_queue);

    template <typename Source, typename T> requires std::is_fundamental<T>::value
    error_code add_source(T, event_queue& = default_queue);

    template <typename... Types>
    void on_event(event_callback_type callback, event_queue& = default_queue)
    {
        default_queue.on_event(get_event_id_for<Types...>(), callback);
    }

    inline void on_event(event_typeid eid, event_callback_type ect) { default_queue.on_event(eid, ect); }

    inline void wait(std::chrono::milliseconds timeout = std::chrono::milliseconds(-1)) { default_queue.wait(timeout); }
    inline void poll() { default_queue.poll(); }

    template <typename EventType>
    error_code send_event(EventType ev) { return default_queue.send_event(get_event_id_for<EventType>(), std::move(ev)); }

    inline error_code send_event(event&& ev) { return default_queue.send_event(ev.type(), std::move(ev)); }
    inline error_code send_event(event_typeid type, event ev) { return default_queue.send_event(type, std::move(ev)); }
}

#endif
