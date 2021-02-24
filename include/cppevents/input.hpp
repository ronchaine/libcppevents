/*!
 *  \brief      basic input event types for libcppevent
 *  \author     Jari Ronkainen
 *  \version    0.6
 */
#ifndef LIBCPPEVENTS_INPUT_HPP
#define LIBCPPEVENTS_INPUT_HPP

#include "event_queue.hpp"
#include "keyboard_codes.hpp"

namespace cppevents::event_groups
{
    struct input : cppevents_group_tag {};
}
namespace cppevents::event
{
    struct input
    {
        using event_group = event_groups::input;
    };

    struct keyboard : input
    {
        enum action_t : uint32_t {
            key_down,
            key_up,
        };

        action_t action;

        uint32_t keyboard_instance = 0;
        kb::scancode scancode;
    };

    struct mouse_button : input
    {
        enum action_t : uint32_t {
            button_down,
            button_up,
        };

        action_t action;

        uint32_t mouse_instance = 0;

        uint32_t click_count = 0;
        uint32_t button = 0;

        int32_t x_pixels = 0;
        int32_t y_pixels = 0;
    };

    struct mouse_motion : input
    {
        uint32_t mouse_instance = 0;

        int32_t x_pixels = 0;
        int32_t y_pixels = 0;

        int32_t x_relative = 0;
        int32_t y_relative = 0;
    };

    struct mouse_wheel : input
    {

        uint32_t mouse_instance = 0;

        int32_t vertical_scroll = 0;
        int32_t horizontal_scroll = 0;
    };

    struct touch : input
    {

        enum action_t : uint32_t {
            press,
            release,
        };

        action_t action;
    };

    struct touch_gesture : input
    {
    };

    struct controller : input
    {
    };
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
