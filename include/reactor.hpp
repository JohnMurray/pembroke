#pragma once

#include <memory>
#include <stdexcept>

extern "C" {
struct event_base;
void event_base_free(event_base *b);
}

namespace pembroke {
    struct ReactorBuilder;

    using reactor_base = std::unique_ptr<event_base, decltype(event_base_free) *>;

    /**
     * @brief Class representing the main event-loop
     * 
     * @see pembroke::ReactorBuilder
     * @see pembroke::reactor()
     */
    class Reactor {
    private:
        reactor_base m_base{nullptr, nullptr};

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

        /**
         * @brief Run the reactor in a non-blocking fashion. This will run all high priority
         *        callbacks on currently ready events and then exit.
         * @note To run continuousy and process events (including those with lower-priority)
         *       use `run_blocking()`
         * @see run_blocking()
         */
        bool run_non_blocking() const;

        /**
         * @brief Run the reactor in a blocking fashion and do not exit.
         * @note In order to exit the event-loop, you will need to call `pause()` from an event
         *       in the reactor. You can continue in a blocking fashion with `resume()`.
         * 
         * @see pause()
         * @see resume()
         */
        bool run_blocking() const;

        bool pause() const;
        bool resume() const;

        // TODO: Does it make sense to have this function AND run_non_blocking()? Semantically what is the difference?
        bool tick() const;

        // TODO: Does this function make sense (combination of flags)
        bool tick_non_blocking() const;
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