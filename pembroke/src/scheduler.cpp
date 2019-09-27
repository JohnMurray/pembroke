#include "scheduler.hpp"

extern "C" {
#include <event2/event.h>
}

namespace pembroke {
    // ---
    // Cancelable Code
    // ---
    Cancelable::Cancelable(std::function<bool()> f) noexcept: m_cancel_cb(f) {}

    bool Cancelable::cancel() const noexcept {
        return m_cancel_cb();
    }

    // ---
    // Scheduler Code
    // ---
    Scheduler::Scheduler(const Reactor *r): m_reactor(r) {}

    std::shared_ptr<Cancelable> Scheduler::schedule(const Task &t,
        const std::chrono::duration<long, std::micro> &delay,
        std::variant<bool, uint32_t> repeat,
        const std::chrono::duration<long, std::micro> &interval) noexcept
    {
        // TODO: register task and index within the class somewhere as a context
        // TODO: provide a reference to this context as the cb arg
        auto task_index = ++m_task_index;
        evtimer_new(nullptr, Scheduler::handle_timer, &task_index);

        // TODO: wrap this all up in a call to the reactor since the `base` is needed
        return nullptr;
    }
}