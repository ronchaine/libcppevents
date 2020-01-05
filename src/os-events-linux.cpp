#include <cppevents/timer.hpp>
#include <cppevents/signal.hpp>

#include <sys/signalfd.h>
#include <sys/timerfd.h>
#include <sys/signal.h>

#include <unistd.h>

#include <unordered_set>

namespace cppevents
{
    static int signal_fd = -1;

    event create_signal_event(int fd)
    {
        signal_event ev;

        struct signalfd_siginfo siginfo;
        ssize_t bytes = read(fd, &siginfo, sizeof(struct signalfd_siginfo));

        if (bytes != sizeof(struct signalfd_siginfo))
            return signal_event{};

        ev.signal_no = siginfo.ssi_signo;
        ev.sender_pid = siginfo.ssi_pid;
        ev.sender_uid = siginfo.ssi_uid;
        ev.trap_no = siginfo.ssi_trapno;
        ev.status = siginfo.ssi_status;

        return std::move(ev);
    }

    error_code os_handle_signal(event_queue& queue, int signal)
    {
        sigset_t mask;
        sigprocmask(SIG_BLOCK, nullptr, &mask);

        if (sigismember(&mask, signal))
            return error_code::already_exists;

        // block the signal from the default handling
        sigaddset(&mask, signal);

        if (sigprocmask(SIG_BLOCK, &mask, nullptr) == -1)
            return error_code::system_error;

        if (signal_fd != -1)
        {
            if (signalfd(signal_fd, &mask, 0) == -1)
                return error_code::system_error;
            else
                return error_code::success;
        }

        signal_fd = signalfd(signal_fd, &mask, 0);

        if (signal_fd == -1)
            return error_code::system_error;

        queue.add_native_source(signal_fd, create_signal_event);

        return error_code::success;
    }
}
