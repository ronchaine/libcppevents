#include <cppevents/events.hpp>

#include <unordered_map>
#include <queue>
#include <deque>

#include <sys/epoll.h>
#include <sys/eventfd.h>

#include <unistd.h>

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

            error_code add_native_source(native_source_type fd, translator_type func, destructor_type);
            void remove_native_source(native_source_type fd);

            error_code send_event(event_typeid, event);

        private:
            std::unordered_map<event_typeid, std::deque<event_callback_type>> events;

            // file descriptor to event translator
            std::unordered_map<int, translator_type> event_translators;
            std::unordered_map<int, destructor_type> event_destructors;

            std::queue<event> sent_events;

            int epoll_fd = -1;
            int notify_fd = -1;
    };

    // event_queue forwarders
    void event_queue::on_event(event_typeid evtype, event_callback_type evcallback) { impl->on_event(evtype, evcallback); }

    void event_queue::wait(std::chrono::milliseconds timeout) { impl->wait(timeout); }
    void event_queue::poll() { impl->poll(); }

    error_code event_queue::add_native_source(native_source_type evdesc, translator_type func, destructor_type rfunc)
    { return impl->add_native_source(evdesc, func, rfunc); }
    void event_queue::remove_native_source(native_source_type evdesc) { return impl->remove_native_source(evdesc); }

    error_code event_queue::send_event(event_typeid type, event ev) { return impl->send_event(type, std::move(ev)); }

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
        ev.events = EPOLLIN;

        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, notify_fd, &ev) == -1)
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
    void event_queue::implementation::on_event(event_typeid evtype, event_callback_type evcall)
    {
        events[evtype].emplace_back(std::move(evcall));
    }

    /**
     * Wait until an event is triggered
     */
    void event_queue::implementation::wait(std::chrono::milliseconds timeout)
    {
        constexpr static int max_events = 16;
        int event_count = 0;

        epoll_event native_event[max_events];

        if (timeout.count() < 0)
            event_count = epoll_wait(epoll_fd, native_event, max_events, -1);
        else
            event_count = epoll_wait(epoll_fd, native_event, max_events, timeout.count());

        for (int i = 0; i < event_count; ++i)
        {
            if (native_event[i].data.fd == notify_fd)
            {
                event ev = std::move(sent_events.front());
                sent_events.pop();
                eventfd_t val;
                eventfd_read(notify_fd, &val);
                for (auto& callback : events[ev.type()])
                    callback(ev);
                continue;
            }

            if (event_translators.count(native_event[i].data.fd) == 0)
                continue;
            if (event_translators[native_event[i].data.fd] == nullptr)
                continue;

            event ev = event_translators[native_event[i].data.fd](native_event[i].data.fd);

            for (auto& callback : events[ev.type()])
                callback(ev);
        }
    }

    /**
     * Poll if an event is triggered
     */
    void event_queue::implementation::poll()
    {
    }

    error_code event_queue::implementation::send_event(event_typeid type, event ev)
    {
        (void)type;

        sent_events.push(std::move(ev));
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
