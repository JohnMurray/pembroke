#include "pembroke/reactor.hpp"
#include "pembroke/internal/util.hpp"

#include <memory>

extern "C" {
#include <event2/event.h>
}

namespace pembroke {

    constexpr int LOOP_RAN_SUCCESSFULLY = 0;
    constexpr int LOOP_RAN_NO_EVENTS = 1;

    constexpr int LOOP_BREAK_SUCCESS = 0;

    // ---
    // REACTOR CONFIGURATION / BUILDER CODE
    // ---

    auto reactor() noexcept -> ReactorBuilder {
        return ReactorBuilder{};
    }

    auto ReactorBuilder::require_edge_trigger_support(bool val) noexcept -> ReactorBuilder & {
        m_require_edge_triggering = val;
        return *this;
    }

    auto ReactorBuilder::require_file_descriptor_support(bool val) noexcept -> ReactorBuilder & {
        m_require_file_descriptor = val;
        return *this;
    }

    auto ReactorBuilder::require_early_close_support(bool val) noexcept -> ReactorBuilder & {
        m_require_early_close = val;
        return *this;
    }

    auto ReactorBuilder::require_order_one_trigger_support(bool val) noexcept -> ReactorBuilder & {
        m_require_order_one_trigger = val;
        return *this;
    }

    auto ReactorBuilder::build() const noexcept -> std::unique_ptr<Reactor> {
        return std::make_unique<Reactor>(*this);
    }

    // ---
    // REACTOR IMPLEMENTATION CODE
    // ---

    Reactor::Reactor(const ReactorBuilder &builder) {
        auto config = std::unique_ptr<
            event_config,
            decltype(event_config_free) *>(event_config_new(), event_config_free);
        int ret = 0;

        if (builder.m_require_edge_triggering) {
            ret = event_config_require_features(config.get(), EV_FEATURE_ET);
            if (ret == -1) {
                throw ConfigurationException("Unable to update reactor config for edge-trigger support");
            }
        }
        if (builder.m_require_file_descriptor) {
            ret = event_config_require_features(config.get(), EV_FEATURE_FDS);
            if (ret == -1) {
                throw ConfigurationException("Unable to update reactor config for file-descriptor support");
            }
        }
        if (builder.m_require_early_close) {
            ret = event_config_require_features(config.get(), EV_FEATURE_EARLY_CLOSE);
            if (ret == -1) {
                throw ConfigurationException("Unable to update reactor config for early support");
            }
        }
        if (builder.m_require_order_one_trigger) {
            ret = event_config_require_features(config.get(), EV_FEATURE_O1);
            if (ret == -1) {
                throw ConfigurationException("Unable to update reactor config for O(1) trigger support");
            }
        }
        // TODO: Configure other features/configs

        m_base = reactor_base(event_base_new_with_config(config.get()), event_base_free);
        if (m_base == nullptr) {
            throw ConfigurationException("Unable to construct reactor with current configuration");
        }
    }

    auto Reactor::run_blocking() const noexcept -> bool {
        return event_base_loop(m_base.get(), EVLOOP_NO_EXIT_ON_EMPTY) == LOOP_RAN_SUCCESSFULLY;
    }

    auto Reactor::stop() const noexcept -> bool {
        return event_base_loopbreak(m_base.get()) == LOOP_BREAK_SUCCESS;
    }

    auto Reactor::tick() const noexcept -> bool {
        if (event_base_get_num_events(m_base.get(), EVENT_BASE_COUNT_ACTIVE) > 0) {
            /* If we know we have active events, pass EVLOOP_ONCE to ensure
             * that we're waiting for current active events to finish and run
             * thier callbacks. */
            return event_base_loop(m_base.get(), EVLOOP_ONCE) == LOOP_RAN_SUCCESSFULLY;
        }
        /* If we don't have any active events, still run the loop, but run with
         * EVLOOP_NONBLOCK, which will not block until any events are active. */
        int ret = event_base_loop(m_base.get(), EVLOOP_NONBLOCK);
        return (ret == LOOP_RAN_NO_EVENTS) || (ret == LOOP_RAN_SUCCESSFULLY);
    }

    auto Reactor::tick_fast() const noexcept -> bool {
        /* Use EVLOOP_NONBLOCK to only execute callbacks for high-priority,
         * ready events */
        int ret = event_base_loop(m_base.get(), EVLOOP_NONBLOCK);
        return (ret == LOOP_RAN_SUCCESSFULLY) || (ret == LOOP_RAN_NO_EVENTS);
    }

    
    auto Reactor::register_event(pembroke::Event &event) noexcept -> bool {
        return event.register_event(*m_base);
    }


} // namespace pembroke