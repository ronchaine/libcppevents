/*!
 *  \file       common.hpp
 *  \brief      common types and constants for libcppevents
 *  \author     Jari Ronkainen
 *  \version    0.2
 */
#ifndef LIBCPPEVENT_COMMON_HPP
#define LIBCPPEVENT_COMMON_HPP

#include <cstdint>
#include <functional>

namespace cppevents::source
{
    using unspecified = void;
    struct timer {};
}

namespace cppevents
{
    struct event_details
    {
        using id_type = uint64_t;

        id_type group_id;
        id_type event_id;

        constexpr auto operator<=>(const event_details&) const noexcept = default;
    };

    /*!
     *  \brief type for empty events
     *
     *  empty_event is a special return value from translator functions,
     *  when it is the *only* event type received, the 'wait'-command
     *  does not get unblocked
     */
    struct empty_event{};

    /*
     * derive from this to mark something as a group
     */
    struct cppevents_group_tag {};

    template <typename T>
    struct is_group : std::is_base_of<cppevents_group_tag, T> {};


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

namespace std
{
    template <>
    struct hash<cppevents::event_details>
    {
        size_t operator()(const cppevents::event_details& ed) const {
            constexpr static size_t prime = 2027;
            size_t hash_value = hash<uint64_t>{}(ed.group_id);

            hash_value *= prime;
            hash_value += hash<uint64_t>{}(ed.event_id);

            return hash_value;
        }
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
