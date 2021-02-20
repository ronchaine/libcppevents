// Example for SDL events
#include <cppevents/sdl2.hpp>
#include <iostream>

int main()
{
    // The first step is just basic SDL initialisation,
    // as you would do it normally
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = nullptr;
    window = SDL_CreateWindow("cppevents SDL example", 200, 200, 0, 0, 0);

    if (!window)
    {
        std::cout << "can't create window\n";
        return -1;
    }

    SDL_Renderer* renderer = nullptr;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cout << "can't get renderer " << SDL_GetError() << "\n";
        return -1;
    }

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Here we add SDL window as an event source, after this
    // libcppevents can handle events coming from SDL,

    // this is equivalent to just cppevents::add_source(window),
    // but the template argument is shown for completeness
    cppevents::add_source<cppevents::source::unspecified>(window);


    //  Create a lambda function for handling keyboard events,
    //  this might as well be a normal function
    auto keyboard_handler = [&](cppevents::raw_event& raw) {
        // Couldn't figure out a way to get rid of this cast,
        // even though it's redundant, sorry.
        auto event = event_cast<cppevents::keyboard_event>(raw);

        std::cout << "Key " << static_cast<uint32_t>(event.scancode) << " ";
        if (event.action == cppevents::keyboard_event::key_down)
            std::cout << "pressed\n";
        else
            std::cout << "released\n";

        if (event.scancode == cppevents::kb::scancode::key_esc)
        {
            SDL_Quit();
            std::exit(0);
        }
    };

    // tell that when a keyboard event happens, we want it to be sent
    // to the keyboard_handler function
    cppevents::on_event<cppevents::keyboard_event>(keyboard_handler);

    // using the lambda directly with mouse motion events
    cppevents::on_event<cppevents::mouse_motion>([&](cppevents::raw_event& raw){
        auto event = event_cast<cppevents::mouse_motion>(raw);
        std::cout << "Mouse moved by " << event.x_relative << "," << event.y_relative << " to " << event.x_pixels << "," << event.y_pixels << "\n";
    });

    // ditto with buttons
    cppevents::on_event<cppevents::mouse_button>([&](cppevents::raw_event& raw){
        auto event = event_cast<cppevents::mouse_button>(raw);

        std::cout << "Mouse " << event.mouse_instance << " button " << static_cast<uint32_t>(event.button) << " ";
        if (event.action == cppevents::mouse_button::button_down)
            std::cout << "pressed ";
        else
            std::cout << "released ";

        std::cout << "at " << event.x_pixels << "," << event.y_pixels << "\n";
    });

    SDL_RenderPresent(renderer);
    SDL_UpdateWindowSurface(window);
    while(true)
    {
        // here we wait until we get an event
        // and launch the appropriate handler when
        // something happens
        cppevents::wait();
        SDL_RenderPresent(renderer);
    }
}
