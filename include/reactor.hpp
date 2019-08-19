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
     * @brief 
     */
    class Reactor {
    private:
        reactor_base m_base{nullptr, nullptr};

    public:
        Reactor(const ReactorBuilder &builder);
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

    // TODO: Below example uses `.enable_multithreaded()` which doesn't exist, update when there is
    //       a better example to use
    /**
     * @brief Simple utility function to create a ReactorBuilder instance
     * Example:
     *     auto reactor = pembroke::reactor()
     *         .require_edge_trigger_support()
     *         .enable_multithreaded()
     *         .build();
     * 
     * @returns A `ReactorBuilder` object
     */
    [[nodiscard]]
    ReactorBuilder reactor() noexcept;
} // namespace pembroke