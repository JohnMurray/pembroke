#include "pembroke/scheduler.hpp"

namespace pembroke {

    Scheduler::Scheduler(const Reactor *r): m_reactor(r) {}

    std::shared_ptr<Event> schedule(const Task &t,
        const timespan &delay,
        bool do_repeat,
        uint32_t repeat_count,
        const timespan &interval,
        std::function<bool()> until_op) noexcept {

        // TODO: implement

        /*
         * 1. create event for our handler function
         * 2. store event into internal stores
         * 3. schedule callback to our handler function
         * 4. construct an Event cancellable
         */
        // TODO: Ensure that I should _only_ be setting `tv_usec` and that this casting
        //       is correct. Feels like it should set both sec and usec and not case from
        //       a long to an int.
        timeval tv {.tv_usec = static_cast<int>(delay.count()) };

        return nullptr;
    }

}