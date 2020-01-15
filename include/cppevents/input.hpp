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
        scancode scancode;
    };

    struct mouse_button
    {
        enum subtype : unsigned int {
            button_down,
            button_up,
        };

        subtype type;
        uint32_t click_count = 0;
    };

    struct mouse_motion
    {
    };

    struct mouse_wheel
    {
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
