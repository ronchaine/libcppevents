#include <cppevents/filesystem.hpp>
#include <iostream>

int main(int argc, char* argv[])
{
    cppevents::event_queue events;

/*
    events.on_event<cppevents::filesystem_event>([](cppevents::event& raw_event){
        auto event = cppevents::event_cast<cppevents::filesystem_event>(raw_event);
        switch (event.type)
        {
        }
    });

    while(true)
    {
        events.wait();
    }
*/
    return 0;
}

