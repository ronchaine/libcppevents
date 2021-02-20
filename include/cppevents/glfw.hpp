#ifndef LIBCPPEVENTS_GLFW_HPP
#define LIBCPPEVENTS_GLFW_HPP

#include <epoxy/gl.h>
#include <GLFW/glfw3.h>

#include "input.hpp"
#include "window.hpp"
#include "event_queue.hpp"

#include <iostream>

namespace cppevents
{
    native_source_type get_glfw_event_source(GLFWwindow* window);

    /*
    template <> error_code add_source<cppevents::input, SDL_Window*>(
        SDL_Window* window,
        event_queue& queue)
    {
    }
    */
}

#endif
