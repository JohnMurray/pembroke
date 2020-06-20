#include "pembroke/event/delayed.hpp"

#include "pembroke/internal/logging.hpp"
#include "pembroke/internal/util.hpp"

extern "C" {
#include <event2/event.h>
}

namespace pembroke::event {

    DelayedEvent::~DelayedEvent() {
        close_timer();
    }

    DelayedEvent::DelayedEvent(DelayedEvent &&event) noexcept 
        : m_delay(event.m_delay),
          m_callback(std::move(event.m_callback)),
          m_timer_event(event.m_timer_event),
          m_canceled(event.m_canceled) {
        event.m_timer_event = nullptr;
    }

    auto DelayedEvent::operator=(DelayedEvent &&event) noexcept -> DelayedEvent& {
        m_delay = event.m_delay;
        m_callback = std::move(event.m_callback);
        m_timer_event = event.m_timer_event;
        m_canceled = event.m_canceled;

        event.m_timer_event = nullptr;
        return *this;
    }


    [[nodiscard]]
    auto DelayedEvent::register_event(event_base &base) noexcept -> bool {
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
        m_timer_event = evtimer_new(&base, DelayedEvent::run_timer_cb, this);
        int ret = evtimer_add(m_timer_event, &tv);

        return ret == 0;
    }

    auto DelayedEvent::cancel() noexcept -> bool {
        return close_timer();
    }

    auto DelayedEvent::canceled() noexcept -> bool {
        return m_canceled;
    }

    auto DelayedEvent::close_timer() noexcept -> bool {
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

    void DelayedEvent::run_timer_cb(int /*unused*/, short /*unused*/, void* cb) noexcept {
        ASSERT_RELEASE(cb != nullptr, "Timer event called with null timer object");
        auto *self = static_cast<DelayedEvent *>(cb);
        if (self->m_timer_event != nullptr) {
            self->m_callback();
            self->close_timer();
        }
    }

} // namespace pembroke::event