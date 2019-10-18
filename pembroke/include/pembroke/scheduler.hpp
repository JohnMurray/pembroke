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

    const auto no_delay = 0s;

    /**
     * @brief A handle to cancel tasks that have been scheduled on a reactor
     */
    class Cancelable {
    private:
        /** @brief Callback provided at construction, to be invoked by `cancel()` */
        std::function<bool()> m_cancel_cb;
    public:

        Cancelable(std::function<bool()>) noexcept;
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
        uint32_t m_task_index{0};
        std::unordered_map<int, int> m_timers{};

        Scheduler(const Reactor *r);
    
    public:
        [[nodiscard]]
        std::shared_ptr<Cancelable> schedule_once(const Task &t, const std::chrono::duration<long, std::micro> &delay) noexcept;

    private:
        /**
         * @brief Submit a task to run on a specific schedule configuration.
         * 
         * All scheduling functions funnel into here and the public interface exposes a much
         * more user-friendly interface to interact with.
         */
        [[nodiscard]]
        std::shared_ptr<Cancelable> schedule(const Task &t,
            const std::chrono::duration<long, std::micro> &delay,
            std::variant<bool, uint32_t> repeat,
            const std::chrono::duration<long, std::micro> &interval) noexcept;

        static void handle_timer(int, short, void *arg) noexcept;
    };

    void Scheduler::handle_timer(int, short, void *arg) noexcept {

    }

} // namespace pembroke