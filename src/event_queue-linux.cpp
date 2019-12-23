#include <cppevents/events.hpp>

#include <unordered_map>
#include <deque>

#include <sys/epoll.h>
#include <unistd.h>

#include <iostream>

namespace cppevents
{
    class event_queue::implementation
    {
        public:
            implementation();
            ~implementation();

            void on_event(event_typeid, event_callback_type);

            void wait(std::chrono::milliseconds timeout);
            void poll();

            error_code add_native_source(native_source_type fd, translator_type func);
            void remove_native_source(native_source_type fd);

        private:
            std::unordered_map<event_typeid, std::deque<event_callback_type>> events;

            // file descriptor to event translator
            std::unordered_map<int, translator_type> event_translators;

            int epoll_fd = -1;

            bool has_events = false;
    };

    // event_queue forwarders
    void event_queue::on_event(event_typeid evtype, event_callback_type evcallback) { impl->on_event(evtype, evcallback); }

    void event_queue::wait(std::chrono::milliseconds timeout) { impl->wait(timeout); }
    void event_queue::poll() { impl->poll(); }

    error_code event_queue::add_native_source(native_source_type evdesc, translator_type func) { return impl->add_native_source(evdesc, func); }
    void event_queue::remove_native_source(native_source_type evdesc) { return impl->remove_native_source(evdesc); }


    // Actual implementation
    event_queue::event_queue() : impl(std::make_unique<implementation>()) {}
    event_queue::~event_queue() = default;

    event_queue::implementation::implementation()
    {
        epoll_fd = epoll_create1(0);
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
    void event_queue::implementation::on_event(event_typeid evtype, event_callback_type evcall)
    {
        events[evtype].emplace_back(std::move(evcall));
    }

    /**
     * Wait until an event is triggered
     */
    void event_queue::implementation::wait(std::chrono::milliseconds timeout)
    {
        if (!has_events)
            return;

        constexpr static int max_events = 16;
        int event_count = 0;

        epoll_event native_event[max_events];

        if (timeout.count() < 0)
            event_count = epoll_wait(epoll_fd, native_event, max_events, -1);
        else
            event_count = epoll_wait(epoll_fd, native_event, max_events, timeout.count());

        for (int i = 0; i < event_count; ++i)
        {
            if (event_translators.count(native_event[i].data.fd) == 0)
                continue;
            if (event_translators[native_event[i].data.fd] == nullptr)
                continue;

            event ev = event_translators[native_event[i].data.fd](native_event[i].data.fd);

            for (auto& callback : events[ev.type()])
            {
                callback(ev);
            }
        }
    }

    /**
     * Poll if an event is triggered
     */
    void event_queue::implementation::poll()
    {
    }

    /**
     * Add a file description to the epoll queue
     */
    error_code event_queue::implementation::add_native_source(native_source_type fd, translator_type func)
    {
        has_events = true;

        epoll_event ev;
        memset(&ev, 0, sizeof(ev));

        ev.events = EPOLLIN;
        ev.data.fd = fd;

        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev))
            return error_code::system_error;

        event_translators[fd] = func;
        return error_code::success;
    }

    /**
     *
     */
    void event_queue::implementation::remove_native_source(native_source_type fd)
    {
        event_translators.erase(fd);
        if (event_translators.empty())
            has_events = false;
    }
}
