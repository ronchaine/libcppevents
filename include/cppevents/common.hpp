#ifndef LIBCPPEVENT_COMMON_HPP
#define LIBCPPEVENT_COMMON_HPP

#include <cstdint>

namespace cppevents
{
    using event_typeid = uint64_t;

    constexpr static int UNINITIALISED_FILE_DESCRIPTOR  = -2;

    template <typename T>
    class badge
    {
        friend T;
        badge() {}
    };

    enum class error_code : int32_t
    {
        success         = 0,
        system_error    = 1,
        already_exists  = 2,
    };
}

#endif
