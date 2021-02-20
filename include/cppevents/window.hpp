#ifndef LIBCPPEVENTS_WINDOW_HPP
#define LIBCPPEVENTS_WINDOW_HPP

#include "events.hpp"
#include "input.hpp"

namespace cppevents::event_group
{
    struct window {};
}

namespace cppevents
{
    struct window_event
    {
        using group = event_group::window;
    };

    struct window_moved
    {
    };

//    struct window_event
//    {
//        enum subtype : uint32_t
//        {
//            move,
//            close,
//            resize,
//            focus_gained,
//            focus_lost,
//        };
//
//        subtype type;
//
//        uint32_t window_id;
//
//        int32_t x;
//        int32_t y;
//    };
}

#endif
