#include <cppevent/filesystem.hpp>

#include <sys/inotify.h>

namespace cppevents
{
    class event_source<filesystem_event>
    {
    };
}
