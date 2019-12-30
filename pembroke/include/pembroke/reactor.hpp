#pragma once

#include <chrono>
#include <memory>
#include <stdexcept>
#include <unordered_set>

#include "pembroke/event.hpp"
#include "pembroke/internal/forward_decls.hpp"

namespace pembroke {
    struct ReactorBuilder;

    using reactor_base = std::unique_ptr<event_base, decltype(event_base_free) *>;
    using event_ptr = std::unique_ptr<event, decltype(event_free) *>;

    /**
     * @brief Class representing the main event-loop
     * 
     * @see pembroke::ReactorBuilder
     * @see pembroke::reactor()
     */
    class Reactor {
        friend class Scheduler;
    private:
        reactor_base m_base{nullptr, nullptr};
        std::unordered_set<std::shared_ptr<EventContext>, EventContextHash> m_timers;

        uint32_t m_event_index = 0;

    public:
        /**
         * @brief Construct a new reactor object given a builder containing configuration information
         * 
         * @see pembroke::reactor()
         * @see pembroke::ReactorBuilder
         * @note Throws and exception if the event-loop fails to construct, which would likely be a
         *       product of any custom configuration settings given. Unfortunately exact details are
         *       not available on _which_ configuration caused the issue due to insufficient error
         *       reporting in libevent.
         * 
         * @param builder ReactorBuilder object constructed from pembroke::reactor()
         */
        Reactor(const ReactorBuilder &builder);

        ~Reactor() = default;

        Reactor(const Reactor &r) = delete;
        Reactor(Reactor &&r) = delete;

        Reactor &operator=(const Reactor &r) = delete;
        Reactor &operator=(Reactor &&r) = delete;

        /**
         * @brief Run the reactor in a blocking fashion and do not exit.
         * @note In order to exit the event-loop, you will need to call `pause()` from an event
         *       in the reactor. You can continue in a blocking fashion by re-calling this method.
         * 
         * @see pause()
         * @returns True if the loop ran successfully, False otherwise
         */
        bool run_blocking() const noexcept;

        /**
         * @brief Run the event-loop once. If there are no active events then this mehtod will
         *        return immediately. If there are active events, then tick() will wait for them
         *        to finish and execute the callbacks before returning.
         * 
         * @note If you only want to execute ready-events, use `tick_fast()`
         * @see tick_fast()
         * @returns True if the loop ran successfully, False otherwise
         */
        bool tick() const noexcept;

        /**
         * @brief Run the event-loop once, much like tick(). However this function will only execute
         *        the callbacks of events that are ready.
         * 
         * @note Not all of the ready-events may be executed. To execute them all, use `tick()`
         * @see tick()
         * @returns True if the loop ran successfully, False otherwise
         */
        bool tick_fast() const noexcept;

        /**
         * @brief Stop the reactor, returning control to the portion of code that invoked the blocking
         *        run (`blocking_run()`). If the reactor is not running, this function will have no
         *        success.
         * @see blocking_run()
         * @return True if the reactor was successfully stopped (or if the reactor was not runnig), False
         *         if an error was encountered while attempting to stop.
         */
        bool stop() const noexcept;


        /**
         * @brief Schedule some call-back to run after a short duration
         * 
         * @note This function is rather low-level/simple and is especially useful
         *       if you are looking to build additional functionality on top of it.
         *       If you're looking for general purpose task scheduling, take a look
         *       at `scheduler`.
         * 
         * @see scheduler
         * 
         * @param cb     The callback to run once the timer expires
         * @param delay  The time to delay before running the callback
         * 
         * @returns Event object which can be used to cancel the event before it
         *          is executed.
         */
        const Event new_timer(
            const std::function<void()> &cb,
            const std::chrono::duration<long, std::micro> &delay) noexcept;
        
    private:

        /** 
         * @brief Helper function used by `new_timer` in executing the user-supplied
         *        callback within libevent.
         *  @see new_timer
         */
        static void run_timer_cb(int, short, void *);

    };

    /** 
     * @brief Simple configuration exception thrown when creating objects from their builders
     * @see ReactorBuilder
     */
    struct ConfigurationException: public std::runtime_error {
        ConfigurationException(const char *what) throw(): std::runtime_error(what){}
    };

    /**
     * @brief A simple builder/configurator for creating a Reactor
     * @see Reactor
     * @see reactor
     */
    struct ReactorBuilder {
        bool m_require_edge_triggering = false;
        bool m_require_file_descriptor = false;
        bool m_require_early_close = false;
        bool m_require_order_one_trigger = false;

        ReactorBuilder &require_edge_trigger_support(bool val = true) noexcept;
        ReactorBuilder &require_file_descriptor_support(bool val = true) noexcept;
        ReactorBuilder &require_early_close_support(bool val = true) noexcept;
        ReactorBuilder &require_order_one_trigger_support(bool val = true) noexcept;

        [[nodiscard]]
        std::unique_ptr<Reactor> build() const noexcept;
    };

    /**
     * @brief Simple utility function to create a ReactorBuilder instance
     * Example:
     *     auto reactor = pembroke::reactor()
     *         .require_edge_trigger_support()
     *         .require_order_one_trigger_support()
     *         .build();
     * 
     * @returns A `ReactorBuilder` object
     */
    [[nodiscard]]
    ReactorBuilder reactor() noexcept;
} // namespace pembroke