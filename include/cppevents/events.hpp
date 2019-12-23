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

namespace cppevents
{
    namespace detail { inline std::atomic<event_typeid> event_typeid_counter = 0; }

    class event;
    class event_queue;

    using event_callback_type = std::function<void(event&)>;

    // tag for enabling all events
    struct any_event{};

    // file descriptor for POSIX
    using native_source_type = int;
    using translator_type = event(*)(native_source_type);

    class event
    {
        public:
            event_typeid type() const noexcept { return id; }

        protected:
            explicit event(event_typeid id) noexcept : id(id) {}

            std::array<std::byte, 16> data;

        private:
            event_typeid id;
    };

    template <typename T, typename... Ts>
    event_typeid get_event_id_for()
    {
        static_assert(std::is_base_of<event, typename std::decay<T>::type>::value);
        static event_typeid ids = 1ul << detail::event_typeid_counter++;

        if constexpr(sizeof...(Ts))
            return ids | get_event_id_for<Ts...>();

        return ids;
    }

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
            error_code add_native_source(native_source_type, translator_type);
            void remove_native_source(native_source_type);

        private:
            class implementation;
            std::experimental::propagate_const<std::unique_ptr<implementation>> impl;
        };

    inline event_queue default_queue;

    template <typename Source, typename T>
    error_code add_source(const T&, event_queue& = default_queue);

    template <typename... Types>
    void on_event(event_callback_type callback, event_queue& = default_queue)
    {
        default_queue.on_event(get_event_id_for<Types...>(), callback);
    }

    inline void on_event(event_typeid eid, event_callback_type ect) { default_queue.on_event(eid, ect); }

    inline void wait(std::chrono::milliseconds timeout = std::chrono::milliseconds(-1)) { default_queue.wait(timeout); }
    inline void poll() { default_queue.poll(); }
}

#endif
