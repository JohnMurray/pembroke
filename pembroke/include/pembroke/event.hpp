#pragma once

#include <functional>
#include <memory>

#include "internal/forward_decls.hpp"
#include "util/func.hpp"

namespace pembroke {

    /**
     * @brief Represents any event that has been scheduled on the Reactor
     *
     * The sole purpose of the `Event` class is to handle cancellation. The class
     * generically handles different types of events (I/O, timers, etc).
     */
    class Event {
    private:
        std::function<bool()> m_cancel_func;

    public:
        Event(std::function<bool()> m_cancel_func) noexcept;

        /**
         * @brief Cancels the event if it has not already completed and had it's
         *        callback run. If the event has completed, calling `cancel()` will
         *        return True and have not effect.
         * @returns True if successfully canceled, False otherwise
         */
        bool cancel() noexcept;
    };

    /**
     * @brief A companion class to Event that helps with internal bookeeping to any class
     *        that is managing and executing events.
     * @see pembroke::Reactor
     */
    struct EventContext {
        const uint64_t key;                    /**< Unique Identifier for ctx */
        const std::function<void()> callback;  /**< Callback called on `cancel()` */
        const struct event *event = nullptr;   /**< Event object that class represents */

        std::function<void()> post_event_cleanup = util::nop_f;

        EventContext(uint64_t key, std::function<void()> cb) noexcept;
        ~EventContext() = default;

        EventContext(const EventContext &ctx) = delete;
        EventContext(EventContext &&ctx) = delete;

        EventContext &operator=(const EventContext &ctx) = delete;
        EventContext &operator=(EventContext &&ctx) = delete;
    };

    /** @brief Required operator for using EventContext within a set/map */
    inline bool operator==(const EventContext &lhs, const EventContext &rhs) {
        return lhs.key == rhs.key;
    }

    /** @brief Simple hash function to support using EventContext within a set/map */
    struct EventContextHash {
        size_t operator() (const std::shared_ptr<EventContext> &ctx) const;
    };

} // namespace pembroke