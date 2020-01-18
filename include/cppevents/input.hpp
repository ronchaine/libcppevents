#ifndef LIBCPPEVENTS_INPUT_HPP
#define LIBCPPEVENTS_INPUT_HPP

#include "events.hpp"
#include "keyboard_codes.hpp"

namespace cppevents
{
    struct keyboard_event
    {
        enum subtype : unsigned int {
            key_down,
            key_up,
        };

        subtype type;

        uint32_t keyboard_instance = 0;
        scancode scancode;
    };

    struct mouse_button
    {
        enum subtype : unsigned int {
            button_down,
            button_up,
        };

        subtype type;

        uint32_t mouse_instance = 0;

        uint32_t click_count = 0;
        uint32_t button = 0;

        int x_pixels = 0;
        int y_pixels = 0;
    };

    struct mouse_motion
    {
        uint32_t mouse_instance = 0;

        int x_pixels = 0;
        int y_pixels = 0;

        int x_relative = 0;
        int y_relative = 0;
    };

    struct mouse_wheel
    {
        uint32_t mouse_instance = 0;

        int vertical_scroll = 0;
        int horizontal_scroll = 0;
    };

    struct touch
    {
        enum subtype : unsigned int {
            motion,
            press,
            release,
            gesture
        };

        subtype type;
    };

    struct controller_event
    {
        enum subtype : unsigned int {
        };

        subtype type;
    };

    struct input { using event_type = void; };
}

#endif
