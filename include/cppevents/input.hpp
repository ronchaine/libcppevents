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

    struct mouse_event
    {
        enum subtype : unsigned int {
            button_down,
            button_up,
            motion,
            wheel,
            click,
        };

        subtype type;

//        uint32_t button;
//        uint32_t 
    };

    struct touch_event
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
