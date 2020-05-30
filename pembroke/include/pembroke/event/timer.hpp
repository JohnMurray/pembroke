#pragma once

#include <chrono>
#include <functional>

#include "pembroke/event.hpp"
#include "pembroke/libevent/forward_decls.hpp"
#include "pembroke/util.hpp"

namespace pembroke::event {

    /**
     * A repeating event that triggers every user-defined interval
     * 
     * Useful for creating repeating tasks such as maintenance/cleanup tasks, data-updates,
     * metric/log flushing, etc. The timer will start as soon as it is registered with the
     * reactor (or with a delay if specified) and will re-run at the specified interval
     * _after_ completion.
     * 
     * **Example:** If a timer is created to run every 5s and takes 2s to complete,
     * scheduling will look like:
     * 
     *     |**|-----|**|-----|**|-----|**|...
     *     
     *     * == 1s running
     *     - == 1s waiting
     * 
     * The 5s timer does not start until the callback completes. So in this scenario, the times
     * at which a task starts may be 7s apart each time.
     */
    class TimerEvent final
        : public Event,
          public EventCancellation
    {
        duration m_initial_delay;
        duration m_interval;
        std::function<void()> m_callback;
        struct event *m_timer_event = nullptr;
        bool m_canceled = false;
        bool m_first_run = true;

    public:

        /**
         * @brief Construct a Timer that executes ``callback`` immediately and then
         * every ``interval``.
         */
        TimerEvent(duration interval, std::function<void()> callback)
            : m_interval(interval), m_callback(callback), m_initial_delay(no_delay) {}

        /**
         * @brief Construct a timer that execute ``callback`` after an initial duration of
         * ``initial_delay` and then every ``interval``.
         */
        TimerEvent(duration initial_delay, duration interval, std::function<void()> callback)
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