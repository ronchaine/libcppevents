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
}
