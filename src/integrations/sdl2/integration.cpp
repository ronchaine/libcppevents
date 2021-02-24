/*!
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
        switch (sdl_event.window.event)
        {
            case SDL_WINDOWEVENT_CLOSE:
                {
                    event::window_closed wevent;
                    wevent.window_id = sdl_event.window.windowID;
                    send_event(wevent);
                    return;
                }
            case SDL_WINDOWEVENT_SHOWN:
            case SDL_WINDOWEVENT_HIDDEN:
            case SDL_WINDOWEVENT_EXPOSED:
            case SDL_WINDOWEVENT_RESTORED:
            case SDL_WINDOWEVENT_MINIMIZED:
            case SDL_WINDOWEVENT_MAXIMIZED:
                {
                    event::window_visibility wevent;
                    wevent.window_id = sdl_event.window.windowID;
                    wevent.type = sdl_event.type == SDL_WINDOWEVENT_SHOWN ? event::window_visibility::shown :
                                  sdl_event.type == SDL_WINDOWEVENT_HIDDEN ? event::window_visibility::hidden :
                                  sdl_event.type == SDL_WINDOWEVENT_EXPOSED ? event::window_visibility::exposed :
                                  sdl_event.type == SDL_WINDOWEVENT_MAXIMIZED ? event::window_visibility::maximized :
                                  sdl_event.type == SDL_WINDOWEVENT_MINIMIZED ? event::window_visibility::minimized :
                                  event::window_visibility::restored;
                    send_event(wevent);
                    return;
                }
            case SDL_WINDOWEVENT_MOVED:
                {
                    event::window_moved wevent;
                    wevent.window_id = sdl_event.window.windowID;
                    wevent.x = sdl_event.window.data1;
                    wevent.y = sdl_event.window.data2;
                    send_event(wevent);
                    return;
                }
            case SDL_WINDOWEVENT_RESIZED:
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                {
                    event::window_size_change wevent;
                    wevent.window_id = sdl_event.window.windowID;
                    wevent.width = sdl_event.window.data1;
                    wevent.height = sdl_event.window.data2;
                    send_event(wevent);
                    return;
                }
            case SDL_WINDOWEVENT_ENTER:
            case SDL_WINDOWEVENT_LEAVE:
                {
                    event::window_mouse_status wevent;
                    wevent.window_id = sdl_event.window.windowID;
                    wevent.type = sdl_event.type == SDL_WINDOWEVENT_ENTER ?
                        event::window_mouse_status::entered :
                        event::window_mouse_status::exited;
                    send_event(wevent);
                    return;
                }
            case SDL_WINDOWEVENT_FOCUS_GAINED:
            case SDL_WINDOWEVENT_FOCUS_LOST:
                {
                    event::window_focus_change wevent;
                    wevent.window_id = sdl_event.window.windowID;
                    wevent.type = sdl_event.type == SDL_WINDOWEVENT_FOCUS_GAINED ? event::window_focus_change::focus_gained :
                                  sdl_event.type == SDL_WINDOWEVENT_TAKE_FOCUS ? event::window_focus_change::focus_offered :
                                  event::window_focus_change::focus_lost;
                    send_event(wevent);
                    return;
                }
            case SDL_WINDOWEVENT_TAKE_FOCUS:
            case SDL_WINDOWEVENT_HIT_TEST:
            default:
                std::cerr << "ERROR: unhandled SDL window event " << sdl_event.window.event << "\n";
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
                {
                    event::keyboard kbevent;
                    kbevent.action = sdl_event.type == SDL_KEYDOWN ? event::keyboard::key_down : event::keyboard::key_up;
                    if (sdl_event.key.keysym.scancode <= 0xE7)
                    {
                        kbevent.scancode = static_cast<kb::scancode>(sdl_event.key.keysym.scancode);
                    } else {
                        kbevent.scancode = kb::scancode::key_none;
                    }
                    if (kbevent.scancode == kb::scancode::key_none)
                    {
                        std::cerr << "ERROR: unhandled key: " << sdl_event.key.keysym.scancode << "\n";
                    }
                    send_event(kbevent);
                    return;
                }
            case SDL_MOUSEMOTION:
                {
                    event::mouse_motion mevent;
                    mevent.mouse_instance = sdl_event.motion.which;
                    mevent.x_pixels = sdl_event.motion.x;
                    mevent.y_pixels = sdl_event.motion.y;
                    mevent.x_relative = sdl_event.motion.xrel;
                    mevent.y_relative = sdl_event.motion.yrel;
                    send_event(mevent);
                    return;
                }
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                {
                    event::mouse_button mevent;
                    mevent.action = sdl_event.type == SDL_MOUSEBUTTONDOWN ? 
                        event::mouse_button::button_down : 
                        event::mouse_button::button_up;
                    mevent.mouse_instance = sdl_event.button.which;
                    mevent.button = sdl_event.button.button;
                    mevent.click_count = sdl_event.button.clicks;
                    mevent.x_pixels = sdl_event.button.x;
                    mevent.y_pixels = sdl_event.button.y;
                    send_event(mevent);
                    return;
                }
            case SDL_FINGERMOTION:
            case SDL_FINGERDOWN:
            case SDL_FINGERUP:
                {
                    event::touch tevent{};
                    send_event(tevent);
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
    raw_event create_sdl_event(native_source_type fd)
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
    template <> error_code add_source<cppevents::source::unspecified, SDL_Window*>(
        SDL_Window* window,
        event_queue& queue)
    {
        cppevents::native_source_type src = cppevents::get_sdl_event_source(window);
        queue.add_native_source(src, cppevents::detail::create_sdl_event);

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
