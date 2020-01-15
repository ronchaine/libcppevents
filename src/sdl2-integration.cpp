#include <cppevents/sdl2.hpp>
#include <unordered_map>

// wayland
#include <wayland-client.h>
#include <unistd.h>

namespace cppevents::detail
{
    static std::unordered_map<native_source_type, wl_display*> display_map;

    cppevents::event create_sdl_window_event(native_source_type fd)
    {
        SDL_Event ev;
        /*
        if (SDL_PeepEvents(&ev, 1, SDL_PEEKEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT) <= 0)
            assert(0);
        */
        while(SDL_PollEvent(&ev))
        {
            switch (ev.type)
            {
                case SDL_MOUSEMOTION:
                    std::cout << "mouse motion\n";
                    break;
                case SDL_WINDOWEVENT:
                    std::cout << "window event\n";
                    break;
                default:
                    std::cout << ev.type << " type\n";
                    continue;
            }
        }

        (void)fd;
        static int i = 0;
        std::cout << i++ << "\n";
        return window_event{};
    }
}

namespace cppevents
{
    static native_source_type get_sdl2_x11_source(SDL_SysWMinfo& winfo)
    {
        return ConnectionNumber(winfo.info.x11.display);
    }

    static native_source_type get_sdl2_wayland_source(SDL_SysWMinfo& winfo)
    {
        int fd = wl_display_get_fd(winfo.info.wl.display);
        detail::display_map.insert({fd, winfo.info.wl.display});
        return fd;
    }

    native_source_type get_sdl_event_source(SDL_Window* window)
    {
        SDL_SysWMinfo info;
        SDL_VERSION(&info.version);

        if (!SDL_GetWindowWMInfo(window, &info))
            return -1;

        switch(info.subsystem)
        {
            case SDL_SYSWM_X11:
                return get_sdl2_x11_source(info);
            #if SDL_VERSION_ATLEAST(2, 0, 2)
            case SDL_SYSWM_WAYLAND:
                return get_sdl2_wayland_source(info);
            #endif
            default:
                break;
        }

        return 0;
    }
}

