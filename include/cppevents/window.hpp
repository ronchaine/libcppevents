/*!
 *  \brief      basic window event types for libcppevent
 *  \author     Jari Ronkainen
 *  \version    0.1
 */
#ifndef LIBCPPEVENTS_WINDOW_HPP
#define LIBCPPEVENTS_WINDOW_HPP

#include "event_queue.hpp"
#include "input.hpp"

namespace cppevents::event_group
{
    struct window : cppevents_group_tag {};
}

namespace cppevents::event
{
    struct window
    {
        using group = event_group::window;
    };

    struct window_closed : window {
        uint32_t window_id;
    };

    struct window_visibility : window {
        uint32_t window_id;
        enum subtype : uint32_t
        {
            shown,
            hidden,
            restored,
            exposed,
            maximized,
            minimized,
        };

        subtype type;
    };

    struct window_moved : window {
        using group = event_group::window;

        uint32_t window_id;
        int32_t x;
        int32_t y;
    };

    struct window_size_change : window {
        uint32_t window_id;
        int32_t width;
        int32_t height;
    };

    struct window_focus_change : window {
        uint32_t window_id;
        enum subtype : uint32_t
        {
            focus_gained,
            focus_lost,
            focus_offered,
        };
        subtype type;
    };

    struct window_mouse_status : window {
        uint32_t window_id;
        enum subtype : uint32_t
        {
            entered,
            exited,
        };
        subtype type;
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
