/*!
 *  \file       sdl2-integration.cpp
 *  \brief      SDL2 integration for libcppevents
 *  \author     Jari Ronkainen
 *  \version    0.7
 *
 *  Implementations to turn SDL2 events into libcppevents events
 *
 *  \todo Windows support
 *  \todo Cocoa support
 *  \todo Text input support
 */
#include <cppevents/sdl2.hpp>
#include <unordered_map>

// wayland
#include <wayland-client.h>
#include <unistd.h>

namespace cppevents::detail
{
    /*!
     *  \brief  Convert an SDL_Event to cppevents window event
     *
     *  Takes in a SDL_Event and translates it to cppevents
     *  window event, and sends it to concerned entities
     */
    static void translate_sdl_window_event(SDL_Event& sdl_event)
    {
        switch (sdl_event.type)
        {
            case SDL_WINDOWEVENT_CLOSE:
                {
                    window_event event;
                    event.window_id = sdl_event.window.windowID;
                    event.type = event.subtype::close;
                    event.x = 0;
                    event.y = 0;
                    send_event(event);
                    return;
                }
            case SDL_WINDOWEVENT_SHOWN:
            case SDL_WINDOWEVENT_HIDDEN:
            case SDL_WINDOWEVENT_EXPOSED:
            case SDL_WINDOWEVENT_MOVED:
            case SDL_WINDOWEVENT_RESIZED:
            case SDL_WINDOWEVENT_SIZE_CHANGED:
            case SDL_WINDOWEVENT_MINIMIZED:
            case SDL_WINDOWEVENT_MAXIMIZED:
            case SDL_WINDOWEVENT_RESTORED:
            case SDL_WINDOWEVENT_ENTER:
            case SDL_WINDOWEVENT_LEAVE:
            case SDL_WINDOWEVENT_FOCUS_GAINED:
            case SDL_WINDOWEVENT_FOCUS_LOST:
            case SDL_WINDOWEVENT_TAKE_FOCUS:
            case SDL_WINDOWEVENT_HIT_TEST:
            default:
                break;
        }
    }

    /*!
     *  \brief  Convert an SDL_Event to cppevents input event
     *
     *  Takes in a SDL_Event and translates it to cppevents
     *  input event, and sends it to concerned entities
     */
    static void translate_sdl_input_event(SDL_Event& sdl_event)
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
                        event.scancode = static_cast<kb::scancode>(sdl_event.key.keysym.scancode);
                    } else {
                        event.scancode = kb::scancode::key_none;
                    }
                    if (event.scancode == kb::scancode::key_none)
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
                    touch event{};
                    send_event(event);
                    return;
                }
            default:
                break;
        }
    }

    /*!
     *  \brief  callback for SDL events
     *  \return empty event
     *
     *  The callback handles forwarding the event itself, and
     *  as such, always returns an empty event to libcppevents.
     *
     *  This is less error-prone in cases where many events
     *  are received in a small amount of time
     */
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
                case SDL_QUIT:

                    // FIXME: HACK, remove this and do something smarter
                    window_event event;
                    event.window_id = ev.window.windowID;
                    event.type = event.subtype::close;
                    event.x = 0;
                    event.y = 0;
                    send_event(event);

                    return empty_event{};
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
    // Get X11 window fd that we can monitor
    static native_source_type get_sdl2_x11_source(SDL_SysWMinfo& winfo)
    {
        return ConnectionNumber(winfo.info.x11.display);
    }

    // Get wayland window fd that we can monitor
    static native_source_type get_sdl2_wayland_source(SDL_SysWMinfo& winfo)
    {
        return wl_display_get_fd(winfo.info.wl.display);
    }

    /*!
     *  \brief  Get an usable event source from a pointer to SDL window
     *
     *  \param  window  SDL_Window* to get the native source type from
     *  \return cppevents native_source_type that can be used as normal
     */
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

    // Specialise cppevents templates
    template <> error_code add_source<cppevents::window_event, SDL_Window*>(
        SDL_Window* window,
        event_queue& queue)
    {
        std::cout << "adding native sdl source\n";
        cppevents::native_source_type src = cppevents::get_sdl_event_source(window);
        queue.add_native_source(src, cppevents::detail::create_sdl_window_event);

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
