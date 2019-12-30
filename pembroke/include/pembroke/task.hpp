#pragma once

#include <functional>
#include <type_traits>
#include <variant>

#include "util/func.hpp"

namespace pembroke {

    struct TaskContext {

    };

    /**
     * @brief
     * Represents a unit of "work" to be performed. @a Work here is defined as
     * a function (or function-like thing) that is run. Task is an abstraction
     * that takes any number of function like "things" and makes then runnable
     * via Task's @p run method.
     * 
     * @note
     * The task represents a work item, but it is not responsible for how the Task is
     * executed (when, on what thread, by what schedule, etc). By itself, Task is not
     * super useful. It is intended to be used in conjunction with a reactor or
     * scheduler and is mostly a convenience wrapper for the user.
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

        // /**
        //  * @brief Helper function to validate function-like "things" are safe to use with Task
        //  * @tparam Func          The type of the function-like thing provided to Task
        //  * @tparam is_predicate  Boolean literal if the function is a predicate function (returns a bool) or
        //  *                       a normal task function (void return-type). Used to choose the correct validations.
        //  */
        // template<typename Func, bool is_predicate>
        // constexpr void validate_task_func() noexcept {
        //     if constexpr ( std::is_pointer_v<Func> ) {
        //         using FuncDeref = typename std::remove_pointer_t<Func>;
        //         Task::validate_task_func<FuncDeref, is_predicate>();
        //     } else {
        //         // Check that our function-like thing is invocable with no arguments
        //         static_assert( std::is_invocable_v<Func>, "Task given a function-like value that is not invocable with no arguments" );

        //         // Check return-types and that they don't throw
        //         if constexpr ( is_predicate ) {
        //             static_assert( std::is_invocable_r_v<bool, Func>, "Task given a function-like value that does not specify a boolean return type" );
        //         } else {
        //             static_assert( std::is_invocable_r_v<void, Func>, "Task given a function-like value that does not specify a void return type" );
        //         }
        //     }
        //

    public:

        /**
         * @brief
         * TODO: Ensure that Func can also be an r-value reference (same with pre/post-condition setup functions)
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
            // /* Perform some type-level validations */
            // Task::validate_task_func<Func, /*is_predicate=*/false>();

            // /* Capture user-defined function "thing" in a std::function */
            // m_task_function = [f]() -> void {
            //     // TODO: smart-pointer classes not recognized by `is_pointer_v` function
            //     if constexpr ( std::is_pointer_v<Func> ) {
            //         typename std::remove_pointer_t<Func> &f_unwrapped = *f;
            //         std::invoke(f_unwrapped);
            //     } else {
            //         std::invoke(f);
            //     }
            // };
        }

        /* TODO: Rethink the interface for running functions.
         * 
         *   - Do we want to also run pre/post conditions for tasks from here?
         *   - If so, how does that change the interface for the scheduler?
         */
        void run(TaskContext &ctx) const noexcept;

    };
} // namespace pembroke