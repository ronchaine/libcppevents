#include <iostream>

#include <cppevents/timer.hpp>

int main()
{
    using namespace std::chrono_literals;

    int tick_timer = 0; // id

    cppevents::add_source(cppevents::timer{tick_timer, 2222ms, 500});

    auto last = std::chrono::system_clock::now();
    cppevents::on_event<cppevents::event::timer>([&](cppevents::raw_event& raw) {
        auto event = cppevents::event_cast<cppevents::event::timer>(raw);

        auto now = std::chrono::system_clock::now();

        uint64_t ms = std::chrono::duration_cast<std::chrono::microseconds>(now - last).count();
        std::cout << "tick from timer "
                  << event.timer_id << ", "
                  << event.expirations << " expirations"
                  << ", time from last: " << ms << "µs\n";
        last = now;
    });

    while(true)
    {
        cppevents::wait();
    }
}
