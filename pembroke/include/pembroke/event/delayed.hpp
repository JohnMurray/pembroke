#pragma once

#include <chrono>
#include <functional>

#include "pembroke/event.hpp"
#include "pembroke/libevent/forward_decls.hpp"
#include "pembroke/util.hpp"

namespace pembroke::event {

    /**
     * A one-time event that will execute after an initial delay. Represents the most
     * basic event type.
     * 
     * Useful for defering actions, implementing timeout behaviors for asynchronous
     * events, or building more complex event types.
     */
    class DelayedEvent final
        : public Event,
          public EventCancellation
    {
        duration m_delay;
        std::function<void()> m_callback;
        struct event *m_timer_event = nullptr;
        bool m_canceled = false;

    public:

        /**
         * Construct a Delayed event that executes ``callback`` after an initial duration
         * ``delay``.
         */
        DelayedEvent(duration delay, std::function<void()> callback)
            : m_delay(delay), m_callback(callback) {}

        ~DelayedEvent();

        [[nodiscard]]
        bool register_event(event_base &base) noexcept;

        [[nodiscard]]
        bool cancel() noexcept;

        [[nodiscard]]
        bool canceled() noexcept;

    private:
        static void run_timer_cb(int, short, void* cb) noexcept;
        bool close_timer() noexcept;
    };

} // namespace pembroke::Event