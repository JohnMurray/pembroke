#pragma once

#include <functional>
#include <memory>

#include "internal/forward_decls.hpp"
#include "util/func.hpp"

namespace pembroke {

    /**
     * @brief
     *   Represents a scheduled event to the user and provides a mechanism to cancel
     *   the event, assuming it is still pending.
     * 
     *   Event objects may be received when scheduling many types of events (I/O,
     *   timers, etc).
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
     * @brief
     *   A companion class that is of use to any class/method that is responsible for
     *   @a creating events on the Reactor and needs a mechanism to tie Events into the
     *   libevent sub-system.
     * 
     *   The EventContext can be stored in a hash by way of the EventContextHash using
     *   the @p key field of the EventContext.
     * 
     * ~~~~~{.cpp}
     *   // using in unordered set
     *   std::unordered_set<EventContext, EventContextHash> my_events;
     * ~~~~~
     * 
     * @note
     *   If the EventContext object is intended to be stored in a map/set then the context
     *   must be populated with a unique-key. The EventContextHash will simply use this key
     *   (without transformations) to serve as the hash. Failure to set a unique value will
     *   break your hash/map.
     * 
     * @see pembroke::Reactor
     * @see pembroke::EventContextHash
     */
    struct EventContext {
        const uint64_t key;                    /**< Unique Identifier for ctx */
        const std::function<void()> callback;  /**< Callback called on event completion (user defined) */
        const struct event *event = nullptr;   /**< Event object that class represents */

        /** @brief A cleanup method that can be set by the class/method coordinating the event */
        std::function<void()> post_event_cleanup = util::nop_f;

        /**
         * @param key Unique key to identify your event. This value will be used by the
         *            EventContextHash to provide a unique hash-value for maps/sets.
         * @param cb  User-provided callback to be called upon event completion
         */
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

    /**
     * @brief Simple hash function to support using EventContext within a set/map
     */
    struct EventContextHash {
        /**
         * @brief
         * @param ctx  Event context to return a hash for
         */
        size_t operator() (const std::shared_ptr<EventContext> &ctx) const;
    };

} // namespace pembroke