#ifndef LIBCPPEVENT_EVENT_QUEUE_HPP
#define LIBCPPEVENT_EVENT_QUEUE_HPP

#include <chrono>
#include <memory>
#include <experimental/propagate_const>

#include "common.hpp"
#include "event.hpp"

namespace cppevents
{
    // #ifdef linux
    // file descriptor for POSIX
    using native_source_type = int;
    using translator_type = raw_event(*)(native_source_type);
    using destructor_type = void(*)(native_source_type);
    // #endif

    class event_queue
    {
        public:
            using callback_type = std::function<void(raw_event&)>;

            event_queue() noexcept;
            ~event_queue();

            template <typename T, typename... Rest>
            void on_event(callback_type) noexcept;

            void bind_group_to_func(event_details::id_type, callback_type) noexcept;
            void bind_event_to_func(event_details::id_type, callback_type) noexcept;

            void wait(std::chrono::milliseconds timeout = std::chrono::milliseconds(-1));
            void poll();

            template <typename EventType>
            void send_event(EventType ev) { return send_event(get_event_details_for<EventType>(), ev); }
            void send_event(event_details, raw_event);

            
            // for adding new events
            error_code add_native_source(native_source_type, translator_type, destructor_type = nullptr);
            void remove_native_source(native_source_type);

        private:
            class implementation;
            std::experimental::propagate_const<std::unique_ptr<implementation>> impl;
    };

    inline event_queue default_queue;


    template <typename T, typename... Rest>
    void event_queue::on_event(callback_type func) noexcept
    {
        bind_event_to_func(get_event_details_for<T>().event_id, func);

        if constexpr (sizeof...(Rest) > 0)
            on_event<Rest...>(func);
    }

    // Handling event sources
    template <typename Source_Tag, typename T> requires (not std::is_fundamental<T>::value) && (not std::is_pointer<T>::value)
    error_code add_source(T&, event_queue& = default_queue);

    template <typename Source_tag, typename T> requires std::is_fundamental<T>::value || std::is_pointer<T>::value
    error_code add_source(T, event_queue& = default_queue);

    template <typename T> requires (not std::is_fundamental<T>::value) && (not std::is_pointer<T>::value)
    error_code add_source(T& t, event_queue& eq = default_queue) {
        return add_source<source::unspecified>(t, eq);
    }

    template <typename T> requires std::is_fundamental<T>::value || std::is_pointer<T>::value
    error_code add_source(T t, event_queue& eq = default_queue) {
        return add_source<source::unspecified>(t, eq);
    }


    // Acting on events
    template <typename... Types>
    void on_event(event_queue::callback_type func, event_queue& = default_queue) {
        default_queue.on_event<Types...>(func);
    }

    template <typename T> requires requires (T t) { T::event_group; }
    void on_event(event_queue::callback_type func, event_queue& = default_queue) {
        default_queue.bind_group_to_func(get_event_group_id_for<T>(), func);
    }

    inline void wait(std::chrono::milliseconds timeout = std::chrono::milliseconds(-1)) { default_queue.wait(timeout); }
    inline void poll() { default_queue.poll(); }

    // Sending events
    template <typename EventType>
    inline void send_event(EventType ev) { return default_queue.send_event(get_event_details_for<EventType>(), std::move(ev)); }
}

#endif
/*
    Copyright (c) 2021 Jari Ronkainen

    This software is provided 'as-is', without any express or implied warranty.
    In no event will the authors be held liable for any damages arising from the
    use of this software.

    Permission is granted to anyone to use this software for any purpose, including
    commercial applications, and to alter it and redistribute it freely, subject to
    the following restrictions:

    1. The origin of this software must not be misrepresented; you must not claim
       that you wrote the original software. If you use this software in a product,
       an acknowledgment in the product documentation would be appreciated but is
       not required.

    2. Altered source versions must be plainly marked as such, and must not be
       misrepresented as being the original software.

    3. This notice may not be removed or altered from any source distribution.
*/
