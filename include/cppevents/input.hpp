/*!
 *  \file       input.hpp
 *  \brief      basic input event types for libcppevent
 *  \author     Jari Ronkainen
 *  \version    0.6
 */
#ifndef LIBCPPEVENTS_INPUT_HPP
#define LIBCPPEVENTS_INPUT_HPP

#include "events.hpp"
#include "keyboard_codes.hpp"

namespace cppevents::event_group
{
    struct input {};
}
namespace cppevents
{

    struct keyboard_event
    {
        using group = event_group::input;


        enum subtype : uint32_t {
            key_down,
            key_up,
        };

        subtype type;

        uint32_t keyboard_instance = 0;
        kb::scancode scancode;
    };

    struct mouse_button
    {
        enum subtype : uint32_t {
            button_down,
            button_up,
        };

        subtype type;

        uint32_t mouse_instance = 0;

        uint32_t click_count = 0;
        uint32_t button = 0;

        int32_t x_pixels = 0;
        int32_t y_pixels = 0;
    };

    struct mouse_motion
    {
        uint32_t mouse_instance = 0;

        int32_t x_pixels = 0;
        int32_t y_pixels = 0;

        int32_t x_relative = 0;
        int32_t y_relative = 0;
    };

    struct mouse_wheel
    {
        uint32_t mouse_instance = 0;

        int32_t vertical_scroll = 0;
        int32_t horizontal_scroll = 0;
    };

    struct touch
    {
        enum subtype : uint32_t {
            motion,
            press,
            release,
            gesture
        };

        subtype type;
    };

    struct controller_event
    {
        enum subtype : uint32_t {
        };

        subtype type;
    };

    struct input { using event_type = void; };
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
