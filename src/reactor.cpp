#include "reactor.hpp"

#include <memory>

extern "C" {
#include <event2/event.h>
}

namespace pembroke {

    /* ---
     * REACTOR CONFIGURATION CODE
     * --- */

    ReactorBuilder reactor() noexcept {
        return ReactorBuilder{};
    }

    ReactorBuilder &ReactorBuilder::require_edge_trigger_support(bool val) noexcept {
        m_require_edge_triggering = val;
        return *this;
    }

    ReactorBuilder &ReactorBuilder::require_file_descriptor_support(bool val) noexcept {
        m_require_file_descriptor = val;
        return *this;
    }

    ReactorBuilder &ReactorBuilder::require_early_close_support(bool val) noexcept {
        m_require_early_close = val;
        return *this;
    }

    ReactorBuilder &ReactorBuilder::require_order_one_trigger_support(bool val) noexcept {
        m_require_order_one_trigger = val;
        return *this;
    }

    std::unique_ptr<Reactor> ReactorBuilder::build() const noexcept {
        return std::make_unique<Reactor>(*this);
    }

    /* ---
     * REACTOR IMPLEMENTATION CODE
     * --- */

    Reactor::Reactor(const ReactorBuilder &builder) {
        auto config = std::unique_ptr<
            event_config,
            decltype(event_config_free) *>(event_config_new(), event_config_free);
        int ret;

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

    bool Reactor::run_non_blocking() const {
        return event_base_loop(m_base.get(), EVLOOP_NONBLOCK) == 0;
    }

    bool Reactor::run_blocking() const {
        return event_base_loop(m_base.get(), EVLOOP_NO_EXIT_ON_EMPTY) == 0;
    }

    bool Reactor::pause() const {
        return event_base_loopbreak(m_base.get()) == 0;
    }

    bool Reactor::resume() const {
        return event_base_loopcontinue(m_base.get()) == 0;
    }

    bool Reactor::tick() const {
        return event_base_loop(m_base.get(), EVLOOP_ONCE) == 0;
    }

    bool Reactor::tick_non_blocking() const {
        return event_base_loop(m_base.get(), EVLOOP_ONCE | EVLOOP_NONBLOCK) == 0;
    }
} // namespace pembroke