#pragma once

#include <chrono>
#include <functional>
#include <utility>

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
            : m_delay(delay), m_callback(std::move(callback)) {}

        ~DelayedEvent() override;

        DelayedEvent(const DelayedEvent&) = delete;
        DelayedEvent (DelayedEvent &&event) noexcept;

        auto operator=(const DelayedEvent&) -> DelayedEvent& = delete;
        auto operator=(DelayedEvent &&event) noexcept -> DelayedEvent&;

        [[nodiscard]]
        auto register_event(event_base &base) noexcept -> bool override;

        [[nodiscard]]
        auto cancel() noexcept -> bool override;

        [[nodiscard]]
        auto canceled() noexcept -> bool override;

    private:
        static void run_timer_cb(int, short, void* cb) noexcept;
        auto close_timer() noexcept -> bool;
    };

} // namespace pembroke::Event