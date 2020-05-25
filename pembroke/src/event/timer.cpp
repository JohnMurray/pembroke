#include "pembroke/event/timer.hpp"

#include "pembroke/internal/logging.hpp"
#include "pembroke/internal/util.hpp"

extern "C" {
#include <event2/event.h>
}

namespace pembroke::event {

    TimerEvent::~TimerEvent() {
        close_timer();
    }


    [[nodiscard]]
    bool TimerEvent::register_event(event_base &base) noexcept {
        // If we have canceled before registration, fail registration
        if (m_canceled) {
            pembroke::logger::warn("Attempting to register cancled timer-event");
            return false;
        }

        if (m_timer_event != nullptr) {
            pembroke::logger::error("Attempting to register a timer-event twice. "
                "Create a new event or use a ScheduledEvent");
            return false;
        }

        timeval tv = internal::to_timeval(m_delay);
        m_timer_event = evtimer_new(&base, TimerEvent::run_timer_cb, this);
        int ret = evtimer_add(m_timer_event, &tv);

        return ret == 0;
    }

    bool TimerEvent::cancel() noexcept {
        return close_timer();
    }

    bool TimerEvent::canceled() noexcept {
        return m_canceled;
    }

    bool TimerEvent::close_timer() noexcept {
        bool ret = true;
        if (m_timer_event != nullptr) {
            ret = evtimer_del(m_timer_event) == 0;
            event_free(m_timer_event);
            m_timer_event = nullptr;
        }

        // always set internal canceled flag
        m_canceled = true;

        return ret;
    }

    void TimerEvent::run_timer_cb(int, short, void* cb) noexcept {
        ASSERT_RELEASE(cb != nullptr, "Timer event called with null timer object");
        auto self = static_cast<TimerEvent *>(cb);
        if (self->m_timer_event != nullptr) {
            self->m_callback();
            self->close_timer();
        }
    }

} // namespace pembroke::event