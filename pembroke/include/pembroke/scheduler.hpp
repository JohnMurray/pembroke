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
     * @note The scheduler contains a non-owning, const, raw pointer to Reactor. This is because the
     *       Scheduler can only be constructed from within the Reactor and `this` is provided to the
     *       scheduler upon construction. This is necessary for the scheduler to register tasks with
     *       reactor.
     */
    class Scheduler {
        /* To provide Reactor with acess to the private constructor. See @note on class */
        friend Reactor;

    private:
        const Reactor *m_reactor;
        uint32_t m_task_index{0};
        std::unordered_map<int, int> m_timers{};

        Scheduler(const Reactor *r);
    
    public:
        [[nodiscard]]
        std::shared_ptr<Event> schedule_once(const Task &t, const timespan &delay) noexcept;

        [[nodiscard]]
        std::shared_ptr<Event> repeat(const Task &t, const timespan &interval) noexcept;

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
            const timespan &interval) noexcept;

        static void handle_timer(int, short, void *arg) noexcept;
    };

    void Scheduler::handle_timer(int, short, void *arg) noexcept {

    }

} // namespace pembroke