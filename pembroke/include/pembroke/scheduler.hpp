#pragma once

#include <chrono>
#include <functional>
#include <memory>
#include <unordered_map>
#include <variant>

#include "reactor.hpp"
#include "task.hpp"

namespace pembroke {
    using namespace std::chrono_literals;
    using timespan = std::chrono::duration<long, std::micro>;

    constexpr auto no_delay = 0s;

    /**
     * @brief Schedules tasks to be run on the reactor
     * 
     * The scheduler contains a non-owning, const, raw pointer to Reactor. This is because the
     * Scheduler can only be constructed from within the Reactor and `this` is provided to the
     * scheduler upon construction. This is necessary for the scheduler to register tasks with
     * reactor.
     * 
     * The scheduler is a non-copyable, movable type that is available through the reactor
     * interface. Schedulers have a lifetime attached to the reactor and should never exist
     * outside the context of a reactor.
     */
    class Scheduler {
        /* To provide Reactor with acess to the private constructor. See @note on class */
        friend Reactor;

    private:
        const Reactor *m_reactor;
        uint32_t m_task_index{0};
        std::unordered_map<int, int> m_timers{};

        /**
         * @brief Constructor only constructable from within `pembroke::Reactor`.
         * @param r Reactor object for which all schedules are submitted
         */
        Scheduler(const Reactor *r);
    
    public:
        /**
         * @brief Schedule a `Task` to run once with a specific @a delay
         * 
         * @note The time specified as the delay is the _minimum_ amount of time that will
         *       occur before the task is executed. The exact execution time may vary depending
         *       on how the reactor is being run (if at all) and the precision of the underlying
         *       event library (libEvent).
         * 
         * @param t      The task to be scheduled and run
         * @param delay  The time delay before executing a task (in microseconds)
         * @returns      A `std::shared_ptr` to an `Event` which can be used to cancel the scheduled task.
         */
        [[nodiscard]]
        std::shared_ptr<Event> once(const Task &t, const timespan &delay) noexcept;

        /**
         * @brief Schedule a `Task` to run repeatedly with a specific initial @a delay with
         *        an @a interval between executions of the task.
         * 
         * @note The @a interval timer will not start until the task has been completed. So if
         *       an interval is set at 2 seconds and the task takes 1 second. There will be 3
         *       seconds between the start of the first task and the start of the second.
         * 
         * @param t        The task to be scheduled and run
         * @param delay    The time delay before executing a task (in microseconds)
         * @param interval The time between task executions
         */
        [[nodiscard]]
        std::shared_ptr<Event> repeat(const Task &t, const timespan &delay, const timespan &interval) noexcept;

        /**
         * @brief Repeat a task until 
         */
        [[nodiscard]]
        std::shared_ptr<Event> repeat_until(const Task &t, const timespan &delay, const timespan &interval,
                                            const std::function<bool()> &until_op) noexcept;

    private:
        /**
         * @brief Submit a task to run on a specific schedule configuration.
         * 
         * All scheduling functions funnel into here and the public interface exposes a much
         * more user-friendly interface to interact with.
         */
        [[nodiscard]]
        std::shared_ptr<Event> schedule(const Task &t,
            const timespan &delay,
            std::variant<bool, uint32_t> repeat,
            const timespan &interval,
            std::function<bool()> until_op) noexcept;

        static void handle_timer(int, short, void *arg) noexcept;
    };

    void Scheduler::handle_timer(int, short, void *arg) noexcept {

    }

} // namespace pembroke