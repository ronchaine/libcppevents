#include <cppevents/filesystem.hpp>

#include <string>

#include <unistd.h>
#include <fcntl.h>
#include <sys/inotify.h>

namespace cppevents
{
    struct inotify_instance
    {
        inotify_instance();
        ~inotify_instance();

        // this is the thing event_queue needs
        int descriptor = UNINITIALISED_FILE_DESCRIPTOR;
    };

    static inotify_instance notifier;

    inotify_instance::inotify_instance()
    {
        notifier.descriptor = inotify_init();
    }

    inotify_instance::~inotify_instance()
    {
        if (notifier.descriptor >= 0)
            close(notifier.descriptor);
    }

    error_code watch_path(const std::string& path, event_queue& queue)
    {
        // handle this locally
        int new_watch = inotify_add_watch(notifier.descriptor, path.c_str(),
                                IN_ACCESS | IN_ATTRIB | IN_OPEN | IN_CLOSE |
                                IN_CREATE | IN_DELETE | IN_DELETE_SELF |
                                IN_MODIFY | IN_MOVE | IN_MOVE_SELF);

        (void)new_watch;
        (void)queue;

        return error_code::success;
    }
}
