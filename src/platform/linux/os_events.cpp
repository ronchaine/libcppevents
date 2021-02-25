/*!
 *  \file       os-events-linux.cpp
 *  \brief      Linux signal/timer events
 *  \author     Jari Ronkainen
 *  \version    0.9
 *
 *  Implementations for timerfd and signalfd to libcppevents
 *
 *  \todo timerfd
 */
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

    raw_event create_signal_event(int fd)
    {
        event::signal ev;

        struct signalfd_siginfo siginfo;
        ssize_t bytes = read(fd, &siginfo, sizeof(struct signalfd_siginfo));

        if (bytes != sizeof(struct signalfd_siginfo))
            return event::signal{};

        ev.signal_no = siginfo.ssi_signo;
        ev.sender_pid = siginfo.ssi_pid;
        ev.sender_uid = siginfo.ssi_uid;
        ev.trap_no = siginfo.ssi_trapno;
        ev.status = siginfo.ssi_status;

        return ev;
    }

    void delete_signal_event(int fd)
    {
        (void) fd;

        // unblock everything
        sigset_t mask;
        sigprocmask(SIG_BLOCK, nullptr, &mask);
        sigprocmask(SIG_UNBLOCK, &mask, nullptr);

        close(signal_fd);
        signal_fd = -1;
    }

    template <> error_code add_source<cppevents::event::signal, int>(
        int signal,
        event_queue& queue)
    {
        sigset_t mask;
        sigprocmask(SIG_BLOCK, nullptr, &mask);

        if (sigismember(&mask, signal))
            return error_code::already_exists;

        sigaddset(&mask, signal);

        // block the signal from the default handling
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

        queue.add_native_source(signal_fd, create_signal_event, delete_signal_event);

        return error_code::success;
    }


    template <> error_code add_source<cppevents::source::unspecified, timer>(
        timer& window,
        event_queue& queue)
    {
        (void)window;
        (void)queue;
        /*
        cppevents::native_source_type src = cppevents::get_sdl_event_source(window);
        queue.add_native_source(src, cppevents::detail::create_sdl_event);
        */
        return error_code::success;
    }

    template <> error_code add_source<cppevents::source::unspecified, timer>(
        timer&& window,
        event_queue& queue)
    {
        (void)window;
        (void)queue;
        /*
        cppevents::native_source_type src = cppevents::get_sdl_event_source(window);
        queue.add_native_source(src, cppevents::detail::create_sdl_event);
        */
        return error_code::success;
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
