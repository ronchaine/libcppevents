#include <cppevents/sdl2.hpp>

int main()
{
    SDL_Window* window = nullptr;
    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow("cppevents SDL example", 200, 200, 0, 0, 0);
}
