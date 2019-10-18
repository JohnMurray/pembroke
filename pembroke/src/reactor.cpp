#include "pembroke/reactor.hpp"
#include "pembroke/scheduler.hpp"

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

    // ---
    // REACTOR IMPLEMENTATION CODE
    // ---

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

    bool Reactor::run_blocking() const noexcept {
        return event_base_loop(m_base.get(), EVLOOP_NO_EXIT_ON_EMPTY) == LOOP_RAN_SUCCESSFULLY;
    }

    bool Reactor::stop() const noexcept {
        return event_base_loopbreak(m_base.get()) == LOOP_BREAK_SUCCESS;
    }

    bool Reactor::tick() const noexcept {
        if (event_base_get_num_events(m_base.get(), EVENT_BASE_COUNT_ACTIVE) > 0) {
            /* If we know we have active events, pass EVLOOP_ONCE to ensure
             * that we're waiting for current active events to finish and run
             * thier callbacks. */
            return event_base_loop(m_base.get(), EVLOOP_ONCE) == LOOP_RAN_SUCCESSFULLY;
        }
        /* If we don't have any active events, still run the loop, but run with
         * EVLOOP_NONBLOCK, which will not block until any events are active. */
        int ret;
        ret = event_base_loop(m_base.get(), EVLOOP_NONBLOCK);
        return (ret == LOOP_RAN_NO_EVENTS) || (ret == LOOP_RAN_SUCCESSFULLY);
    }

    bool Reactor::tick_fast() const noexcept {
        /* Use EVLOOP_NONBLOCK to only execute callbacks for high-priority,
         * ready events */
        int ret = event_base_loop(m_base.get(), EVLOOP_NONBLOCK);
        return (ret == LOOP_RAN_SUCCESSFULLY) || (ret == LOOP_RAN_NO_EVENTS);
    }

    const Event Reactor::new_timer(
        const std::function<void()> &cb,
        const std::chrono::duration<long, std::micro> &delay) noexcept {

        timeval tv {.tv_usec = static_cast<int>(delay.count()) };
        auto ctx = std::make_shared<EventContext>(++m_event_index, cb);
        auto event = evtimer_new(m_base.get(), Reactor::run_timer_cb, ctx.get());

        /* Configure the context and store in the reactor to reference back to later */
        ctx->event = event;
        ctx->post_event_cleanup = [event, this, ctx]() -> void {
            this->m_timers.erase(ctx);
            event_free(event);
            ctx->event = nullptr;
        };
        m_timers.insert(ctx);

        /* Schedule event to run with provided delay (time-value) */
        evtimer_add(event, &tv);

        /* Return an event object to give the user a way to cancel the event
         * before it is executed. */
        return Event([ctx, this]() -> bool {
            bool ret = true;
            if (ctx->event != nullptr) {
                auto ev = const_cast<struct event*>(ctx->event);
                ret = evtimer_del(ev) == 0;
                event_free(ev);
                this->m_timers.erase(ctx);
            }
            return ret;
        });
    }

    void Reactor::run_timer_cb(int, short, void* cb) {
        auto ctx = static_cast<EventContext *>(cb);
        ctx->callback();
        ctx->post_event_cleanup();
        ctx->post_event_cleanup = util::nop_f;
    }

} // namespace pembroke