#include <cppevents/filesystem.hpp>
#include <iostream>

int main()
{
    cppevents::filesystem_event ev;
    cppevents::dummy_event ev2;

    std::cout << ev.type() << "\n";
    std::cout << ev2.type() << "\n";

    std::cout << cppevents::filesystem_event::event_id << "\n";
}
