#pragma once

#include <chrono>
#include <memory>

#include "reactor.hpp"
#include "task.hpp"

namespace pembroke {

    /**
     * @brief A handle to cancel tasks that have been scheduled on a reactor
     */
    class Cancelable {
    private:
        // TODO: Define some kind of handle that can be used
    public:
        /**
         * @brief Attempts to cancel an event on the reactor
         * 
         * This function may return False for a number of reasons, such as:
         *   - The task was scheduled to run once and has already been executed (nothing to cancel)
         *   - The reactor has been shut-down and cleaned up, nothing to cancel
         *   - An unexpected error was encountered when cancelling the task
         * 
         * @returns True if the operation can be canceled, False otherwise
         */
        [[nodiscard]]
        bool cancel() const noexcept;
    };

    /**
     * @brief Schedules tasks to be run on the reactor
     * 
     * @note The scheduler contains a non-owning, const, raw pointer to Reactor. This is because the
     *       Scheduler can only be constructed from within the Reactor and `this` is provided to the
     *       scheduler upon construction. This is necessary for the scheduler to register tasks with
     *       reactor.
     */
    class Scheduler {
        friend class Reactor;

    private:
        const Reactor *m_reactor;

        Scheduler(const Reactor *r);
    
    public:
        [[nodiscard]]
        std::shared_ptr<Cancelable> schedule_once(const Task &t, const std::chrono::duration<long, std::micro> &delay) noexcept;
    };

} // namespace pembroke