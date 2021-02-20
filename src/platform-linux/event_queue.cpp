/*!
 *  \file       event-queue-linux.cpp
 *  \brief      Event queue implementation for linux/epoll
 *  \author     Jari Ronkainen
 *  \version    0.8
 *
 *  Implementations for the event handling functions for
 *  linux/epoll
 *
 *  If it turns out to be simpler to handle all POSIX-systems
 *  in one file, this might be renamed to -posix and kqueue
 *  and whatever OS X uses added here with ifdefs
 */
#include <cppevents/events.hpp>

#include <unordered_map>
#include <queue>
#include <deque>

#include <sys/epoll.h>
#include <sys/eventfd.h>

#include <unistd.h>

#include <iostream>

namespace cppevents
{
    class event_queue::implementation
    {
        public:
            implementation();
            ~implementation();

            void on_event(event_details, event_callback_type);

            void wait(std::chrono::milliseconds timeout);
            void poll();

            error_code add_native_source(native_source_type fd, translator_type func, destructor_type);
            error_code add_edge_triggered_native_source(native_source_type fd, translator_type func, destructor_type);
            void remove_native_source(native_source_type fd);

            error_code send_event(event_details, raw_event);

        private:
            std::unordered_map<event_details, std::deque<event_callback_type>> events;

            // file descriptor to event translator
            std::unordered_map<int, translator_type> event_translators;
            std::unordered_map<int, destructor_type> event_destructors;

            int epoll_fd = -1;
            int notify_fd = -1;
    };

    // event_queue forwarders
    void event_queue::on_event(event_details evtype, event_callback_type evcallback) { impl->on_event(evtype, evcallback); }

    void event_queue::wait(std::chrono::milliseconds timeout) { impl->wait(timeout); }
    void event_queue::poll() { impl->poll(); }

    error_code event_queue::add_native_source(native_source_type evdesc, translator_type func, destructor_type rfunc)
    { return impl->add_native_source(evdesc, func, rfunc); }
    error_code event_queue::add_edge_triggered_native_source(native_source_type evdesc, translator_type func, destructor_type rfunc)
    { return impl->add_native_source(evdesc, func, rfunc); }
    void event_queue::remove_native_source(native_source_type evdesc) { return impl->remove_native_source(evdesc); }

    error_code event_queue::send_event(event_details type, raw_event ev) { return impl->send_event(type, std::move(ev)); }

    // Actual implementation
    event_queue::event_queue() : impl(std::make_unique<implementation>()) {}
    event_queue::~event_queue() = default;

    event_queue::implementation::implementation()
    {
        epoll_fd = epoll_create1(0);

        // used for messages with no real OS notification
        notify_fd = eventfd(0, 0);

        epoll_event ev{};
        ev.data.fd = notify_fd;
        ev.events = EPOLLIN | EPOLLET;

        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, notify_fd, &ev) == -1) {}
            ;
    }

    event_queue::implementation::~implementation()
    {
        if (epoll_fd > 0)
            ::close(epoll_fd);
    }

    /**
     * Set an action to be performed on an event
     *
     * \param   evtype  type ID for the event for which the action will be triggered
     * \param   evcall  function to be called when the event happens
     */
    void event_queue::implementation::on_event(event_details evtype, event_callback_type evcall)
    {
        events[evtype].emplace_back(std::move(evcall));
    }

    /**
     * Wait until an event is triggered
     */
    void event_queue::implementation::wait(std::chrono::milliseconds timeout)
    {
        all_events_ignored:
        constexpr static int max_events = 16;
        int event_count = 0;
        int ignored_events = 0;

        epoll_event native_event[max_events];

        if (timeout.count() < 0)
            event_count = epoll_wait(epoll_fd, native_event, max_events, -1);
        else
            event_count = epoll_wait(epoll_fd, native_event, max_events, timeout.count());

        for (int i = 0; i < event_count; ++i)
        {
            if (native_event[i].data.fd == notify_fd)
                continue;
            if (event_translators.count(native_event[i].data.fd) == 0)
                continue;
            if (event_translators[native_event[i].data.fd] == nullptr)
                continue;

            raw_event ev = event_translators[native_event[i].data.fd](native_event[i].data.fd);

            if (get_event_details_for<empty_event>() == ev.type())
            {
                ignored_events++;
                continue;
            }

            for (auto& callback : events[ev.type()])
                callback(ev);
        }

        if (ignored_events == event_count)
            goto all_events_ignored;
    }

    /**
     * Poll if an event is triggered
     */
    void event_queue::implementation::poll()
    {
        // FIXME: implementation
    }

    /**
     * Handle sent event directly
     */
    error_code event_queue::implementation::send_event(event_details type, raw_event ev)
    {
        (void)type;
        for (auto& callback : events[ev.type()])
            callback(ev);

        eventfd_write(notify_fd, 1);
        return error_code::success;
    }

    /**
     * Add a file description to the epoll queue
     */
    error_code event_queue::implementation::add_native_source(native_source_type fd,
                                                              translator_type func,
                                                              destructor_type rfunc)
    {
        epoll_event ev{};

        ev.events = EPOLLIN;
        ev.data.fd = fd;

        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev))
            return error_code::system_error;

        event_translators[fd] = func;
        event_destructors[fd] = rfunc;

        return error_code::success;
    }

    /**
     *
     */
    void event_queue::implementation::remove_native_source(native_source_type fd)
    {
        event_translators.erase(fd);
    }
}
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