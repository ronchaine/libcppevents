/*!
 *  \file       common.hpp
 *  \brief      common types and constants for libcppevents
 *  \author     Jari Ronkainen
 *  \version    0.1
 */
#ifndef LIBCPPEVENT_COMMON_HPP
#define LIBCPPEVENT_COMMON_HPP

#include <cstdint>

namespace cppevents
{
    using event_typeid = uint64_t;

    constexpr static int UNINITIALISED_FILE_DESCRIPTOR  = -2;

    struct empty_event{};

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
