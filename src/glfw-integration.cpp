#include <cppevents/glfw.hpp>
#define GLFW_EXPOSE_NATIVE_WAYLAND
#include <GLFW/glfw3native.h>
#include <unordered_map>

// wayland
#include <wayland-client.h>
#include <unistd.h>

namespace cppevents::detail
{
    static std::unordered_map<native_source_type, wl_display*> display_map;

    cppevents::event create_glfw_window_event(native_source_type fd)
    {
        glfwPollEvents();

        (void)fd;
        static int i = 0;
        std::cout << i++ << "\n";
        return window_event{};
    }
}

namespace cppevents
{
    /*
    static native_source_type get_glfw_x11_source()
    {
        return -1;
    }
    */
    static native_source_type get_glfw_wayland_source()
    {
        int fd = wl_display_get_fd(glfwGetWaylandDisplay());
        return fd;
    }

    native_source_type get_glfw_event_source(GLFWwindow* window)
    {
        (void)window;
        return get_glfw_wayland_source();
    }
}

