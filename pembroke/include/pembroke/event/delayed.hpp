#pragma once

#include <chrono>
#include <functional>

#include "pembroke/event.hpp"
#include "pembroke/internal/forward_decls.hpp"

namespace pembroke::event {

    class DelayedEvent final
        : public Event,
          public EventCancellation
    {
        std::chrono::duration<long, std::micro> m_delay;
        std::function<void()> m_callback;
        struct event *m_timer_event = nullptr;
        bool m_canceled = false;

    public:

        DelayedEvent(std::chrono::duration<long, std::micro> delay, std::function<void()> callback)
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