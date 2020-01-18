#include <cppevents/sdl2.hpp>
#include <unordered_map>

// wayland
#include <wayland-client.h>
#include <unistd.h>

namespace cppevents::detail
{
    struct empty_event{};

    void translate_sdl_window_event(SDL_Event& ev)
    {
        (void)ev;
    }

    void translate_sdl_input_event(SDL_Event& sdl_event)
    {
        switch (sdl_event.type)
        {
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                { // make this different scope
                    keyboard_event event;
                    event.type = sdl_event.type == SDL_KEYDOWN ? keyboard_event::key_down : keyboard_event::key_up;
                    if (sdl_event.key.keysym.scancode <= 0xE7)
                    {
                        event.scancode = static_cast<scancode>(sdl_event.key.keysym.scancode);
                    } else {
                        event.scancode = scancode::key_none;
                    }
                    if (event.scancode == scancode::key_none)
                    {
                        std::cerr << "ERROR: unhandled key: " << sdl_event.key.keysym.scancode << "\n";
                    }
                    send_event(event);
                    return;
                }
            case SDL_MOUSEMOTION:
                {
                    mouse_motion event;
                    event.mouse_instance = sdl_event.motion.which;
                    event.x_pixels = sdl_event.motion.x;
                    event.y_pixels = sdl_event.motion.y;
                    event.x_relative = sdl_event.motion.xrel;
                    event.y_relative = sdl_event.motion.yrel;
                    send_event(event);
                    return;
                }
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                {
                    mouse_button event;
                    event.type = sdl_event.type == SDL_MOUSEBUTTONDOWN ? mouse_button::button_down : mouse_button::button_up;
                    event.mouse_instance = sdl_event.button.which;
                    event.button = sdl_event.button.button;
                    event.click_count = sdl_event.button.clicks;
                    event.x_pixels = sdl_event.button.x;
                    event.y_pixels = sdl_event.button.y;
                    send_event(event);
                    return;
                }
            case SDL_FINGERMOTION:
            case SDL_FINGERDOWN:
            case SDL_FINGERUP:
                {
                    touch event;
                    send_event(event);
                    return;
                }
            default:
                break;
        }
    }

    event create_sdl_window_event(native_source_type fd)
    {
        (void)fd;

        SDL_Event ev;
        while(SDL_PollEvent(&ev))
        {
            switch (ev.type)
            {
                case SDL_WINDOWEVENT:
                    translate_sdl_window_event(ev);
                    continue;
                case SDL_CONTROLLERAXISMOTION:
                case SDL_CONTROLLERBUTTONDOWN:
                case SDL_CONTROLLERBUTTONUP:
                case SDL_CONTROLLERDEVICEADDED:
                case SDL_CONTROLLERDEVICEREMOVED:
                case SDL_CONTROLLERDEVICEREMAPPED:
                case SDL_FINGERMOTION:
                case SDL_FINGERDOWN:
                case SDL_FINGERUP:
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                case SDL_JOYAXISMOTION:
                case SDL_JOYBALLMOTION:
                case SDL_JOYHATMOTION:
                case SDL_JOYBUTTONDOWN:
                case SDL_JOYBUTTONUP:
                case SDL_JOYDEVICEADDED:
                case SDL_JOYDEVICEREMOVED:
                case SDL_MOUSEMOTION:
                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP:
                case SDL_MOUSEWHEEL:
                case SDL_MULTIGESTURE:
                    translate_sdl_input_event(ev);
                    continue;
                // TODO: Support IME
                case SDL_TEXTINPUT:
                case SDL_TEXTEDITING:
                    std::cout << "sdl text input\n";
                    continue;
                default:
                    std::cerr << "ERROR: unhandled SDL event type " << ev.type << "\n";
                    continue;
            }
        }

        return empty_event{};
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
        return wl_display_get_fd(winfo.info.wl.display);
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
