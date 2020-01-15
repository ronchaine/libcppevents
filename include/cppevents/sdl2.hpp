#ifndef LIBCPPEVENTS_SDL2_HPP
#define LIBCPPEVENTS_SDL2_HPP

#include <SDL.h>
#include <SDL_syswm.h>

#include "input.hpp"
#include "window.hpp"
#include "events.hpp"

#include <iostream>

namespace cppevents
{
    native_source_type get_sdl_event_source(SDL_Window* window);

    /*
    template <> error_code add_source<cppevents::input, SDL_Window*>(
        SDL_Window* window,
        event_queue& queue)
    {
    }
    */
}

#endif
