/*!
 *  \file       sdl2.hpp
 *  \brief      SDL2 integration header for libcppevents
 *  \author     Jari Ronkainen
 *  \version    1.0
 */
#ifndef LIBCPPEVENTS_SDL2_HPP
#define LIBCPPEVENTS_SDL2_HPP

#include <SDL.h>
#include <SDL_syswm.h>

#include "input.hpp"
#include "window.hpp"
#include "event_queue.hpp"

#include <iostream>

namespace cppevents
{
    native_source_type get_sdl_event_source(SDL_Window* window);
}

#endif
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
