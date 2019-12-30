#pragma once

#include <functional>
#include <type_traits>
#include <variant>

#include "pembroke/util/func.hpp"

namespace pembroke {

    /**
     * @brief
     * TaskContext is a context object that may be optionally consumed by a user-provided
     * callable within the Task class. The context may be populated differently depending
     * on how the Task was created and for what purpose (e.g. delayed event, scheduled
     * task, etc).
     * 
     * @see pembroke::Task
     */
    struct TaskContext {};

    /**
     * @brief
     * Represents a unit of "work" to be performed. @a Work here is defined as
     * a function (or function-like thing) that is run. Task is an abstraction
     * that takes a number of function like "things" and makes then runnable
     * via Task's @p run method.
     * 
     * @note
     * The task represents a work item, but it is not responsible for how the Task is
     * executed (when, on what thread, by what schedule, etc). By itself, Task is not
     * super useful. It is intended to be used in conjunction with a reactor or
     * scheduler and is mostly a convenience wrapper.
     * 
     * @see pembroke::Reactor
     * @see pembroke::Scheduler
     * @see pembroke::TaskContext
     */
    class Task {
    private:
        using TaskFunc = std::function<void()>;
        using TaskFuncWithCtx = std::function<void(TaskContext&)>;

        std::variant<TaskFunc, TaskFuncWithCtx> m_task_function = util::nop_f;


    public:

        /**
         * @brief Create a task object given a callable object `Func` by the user.
         * 
         * Construction will ensure that the callable object matches the expectations
         * of how it will be called and will be stored internally as a std::function.
         * 
         * How or when the function is called is up to the owner of the Task object.
         * 
         * @param f The callable object to be run by the Task
         */
        template<typename Func>
        Task(Func &&f) {
            static_assert(
                // Raise an error if: ![void() || void(TaskContext&)]
                std::disjunction_v<std::is_invocable<Func>, std::is_invocable<Func, TaskContext&>>,
                "Task function-object is not invocable with no-arg or TaskContext&. Invalid Function"
            );

            if constexpr (std::is_invocable_v<Func> ) {
                if constexpr (std::is_same_v<TaskFunc, Func>) {
                    m_task_function = std::forward<Func>(f);
                }
                else {
                    m_task_function = TaskFunc { std::forward<Func>(f) };
                }
            }
            else if constexpr (std::is_invocable_v<Func, TaskContext&>) {
                if constexpr (std::is_same_v<TaskFuncWithCtx, Func>) {
                    m_task_function = std::forward<Func>(f);
                }
                else {
                    m_task_function = TaskFuncWithCtx { std::forward<Func>(f) };
                }
            }
        }

        /**
         * @brief Execute the user-provided function stored within the task
         * 
         * @note While the function is marked noexcept, it is possible that the
         *       user-provided callable may throw. All errors will be caught within
         *       the body of the run function and reported through the internal
         *       logging system. However there will be no other external hooks to
         *       indicate that an exception has been thrown.
         * 
         * @param ctx Context object optionally consumable by the user-provided function
         */
        void run(TaskContext &ctx) const noexcept;
    };

} // namespace pembroke