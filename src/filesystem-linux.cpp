#include <cppevents/filesystem.hpp>

#include <sys/inotify.h>

namespace cppevents
{
    class filesystem_event::implementation
    {
    };

    filesystem_event::filesystem_event() : event(event_id) {}
    filesystem_event::~filesystem_event() = default;

    error_code add_filesystem_watch(const char* path, event_queue&)
    {
        return 0;
    }

    error_code remove_filesystem_watch(const char* path, event_queue&)
    {
        return 0;
    }
}
