/*!
 *  \file       events.hpp
 *  \brief      event queue definitions for libcppevent
 *  \author     Jari Ronkainen
 *  \version    0.7
 *
 *  \todo Windows support
 */
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

    using event_callback_type = std::function<void(raw_event&)>;

    // file descriptor for POSIX
    using native_source_type = int;
    using translator_type = raw_event(*)(native_source_type);
    using destructor_type = void(*)(native_source_type);

    /*!
     *  \brief  Event queue class
     *
     *  A PIMPL wrapper to platform-specific class that handles
     *  the polling and waiting for events, and then firing the
     *  callbacks requested by the user
     */
    class event_queue
    {
        public:
            event_queue();
            ~event_queue();

            template <typename T, typename... Others>
            void on_event(event_callback_type callback)
            {
                on_event(get_event_id_for<T>(), callback);
                if constexpr (sizeof...(Others) > 0)
                    on_event<Others...>(callback);
            }
            void on_event(event_typeid, event_callback_type);

            void wait(std::chrono::milliseconds timeout = std::chrono::milliseconds(-1));
            void poll();

            // This is needed only for adding new events
            error_code add_native_source(native_source_type, translator_type, destructor_type = nullptr);
            error_code add_edge_triggered_native_source(native_source_type, translator_type, destructor_type = nullptr);
            void remove_native_source(native_source_type);

            template <typename EventType>
            error_code send_event(EventType ev) { return send_event(get_event_id_for<EventType>(), ev); }
            error_code send_event(event_typeid, raw_event);

        private:
            class implementation;
            std::experimental::propagate_const<std::unique_ptr<implementation>> impl;
        };

    //! Default event queue type, generally used if no other queue is specified
    inline event_queue default_queue;

    template <typename Source, typename T> requires (not std::is_fundamental<T>::value) && (not std::is_pointer<T>::value)
    error_code add_source(T&, event_queue& = default_queue);

    template <typename Source, typename T> requires std::is_fundamental<T>::value || std::is_pointer<T>::value
    error_code add_source(T, event_queue& = default_queue);

    template <typename... Types>
    void on_event(event_callback_type callback, event_queue& = default_queue)
    {
        default_queue.on_event<Types...>(callback);
    }

    inline void on_event(event_typeid eid, event_callback_type ect) { default_queue.on_event(eid, ect); }

    inline void wait(std::chrono::milliseconds timeout = std::chrono::milliseconds(-1)) { default_queue.wait(timeout); }
    inline void poll() { default_queue.poll(); }

    template <typename EventType>
    error_code send_event(EventType ev) { return default_queue.send_event(get_event_id_for<EventType>(), std::move(ev)); }

    inline error_code send_event(raw_event&& ev) { return default_queue.send_event(ev.type(), std::move(ev)); }
    inline error_code send_event(event_typeid type, raw_event ev) { return default_queue.send_event(type, std::move(ev)); }
}

#endif
/*
    Copyright (c) 2020 Jari Ronkainen

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
