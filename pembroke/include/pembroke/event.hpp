#pragma once

#include <chrono>
#include <functional>
#include <memory>

#include "pembroke/internal/forward_decls.hpp"
#include "pembroke/internal/lang_macros.hpp"

namespace pembroke {

    class Event {
    public:
        virtual ~Event() = default;

        [[nodiscard]]
        virtual bool register_event(event_base &base) noexcept PURE;
    };

    class EventCancellation {
        [[nodiscard]]
        virtual bool cancel() noexcept PURE;

        [[nodiscard]]
        virtual bool canceled() noexcept PURE;
    };

} // namespace pembroke