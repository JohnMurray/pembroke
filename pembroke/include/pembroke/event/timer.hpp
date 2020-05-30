#pragma once

#include <chrono>
#include <functional>

#include "pembroke/event.hpp"
#include "pembroke/internal/forward_decls.hpp"
#include "pembroke/util.hpp"

namespace pembroke::event {

    /**
     * @brief A repeating event that triggers every user-defined interval
     * 
     * Useful for creating repeating tasks such as maintenance/cleanup tasks, data-updates,
     * metric/log flushing, etc. The timer will start as soon as it is registered with the
     * reactor and will re-run at a specified interval _after_ completion.
     * 
     * Ex: If a timer is created to run every 5s and takes 2s to complete, scheduling will
     *     look like:
     *       |**|-----|**|-----|**|-----|**|...
     *     The 5s timer does not start until the callback completes.
     */
    class TimerEvent final
        : public Event,
          public EventCancellation
    {
        std::chrono::duration<long, std::micro> m_initial_delay;
        std::chrono::duration<long, std::micro> m_interval;
        std::function<void()> m_callback;
        struct event *m_timer_event = nullptr;
        bool m_canceled = false;
        bool m_first_run = true;

    public:

        TimerEvent(std::chrono::duration<long, std::micro> interval, std::function<void()> callback)
            : m_interval(interval), m_callback(callback), m_initial_delay(no_delay) {}

        TimerEvent(std::chrono::duration<long, std::micro> initial_delay,
            std::chrono::duration<long, std::micro> interval,
            std::function<void()> callback)
            : m_interval(interval), m_initial_delay(initial_delay), m_callback(callback) {}

        ~TimerEvent();

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