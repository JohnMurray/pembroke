#pragma once

#include <chrono>
#include <functional>
#include <memory>

#include "pembroke/libevent/forward_decls.hpp"
#include "pembroke/internal/lang_macros.hpp"

namespace pembroke {

    class Event {
    public:
        virtual ~Event() = default;

        [[nodiscard]]
        virtual auto register_event(event_base &base) noexcept -> bool PURE;
    };

    class EventCancellation {

        /**
         * @brief Cancels the event. Callbacks on the event should not run after
         * cancel has been called (assuming cancellation suceeds).
         */
        [[nodiscard]]
        virtual auto cancel() noexcept -> bool PURE;

        /**
         * @brief Returns bool indicating if the event has been canceled.
         * @note Does not take into account event's registration status.
         */
        [[nodiscard]]
        virtual auto canceled() noexcept -> bool PURE;
    };

} // namespace pembroke