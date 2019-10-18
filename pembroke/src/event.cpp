#include "pembroke/event.hpp"

#include <exception>
#include <sstream>

#include "pembroke/internal/logging.hpp"

namespace pembroke {

    Event::Event(std::function<bool()> cf) noexcept:
        m_cancel_func(cf) {}

    bool Event::cancel() noexcept {
        try {
            return m_cancel_func();
        } catch (const std::exception &ex) {
            std::stringstream ss;
            ss << "Uncaught exception encountered while cancelling event: "
               << ex.what();
            logger::error(ss.str());
        } catch (...) {
            logger::error(
                "Uncaught exceptionof an unknown type encountered "
                "(not defined within class heirarchy of std::exception) "
                "while cancelling event");
        }

        return false;
    }

    EventContext::EventContext(uint64_t key, std::function<void()> cb) noexcept:
        key(key), callback(cb) {}

    size_t EventContextHash::operator() (const std::shared_ptr<EventContext> &ctx) const {
        return static_cast<size_t>(ctx->key);
    }

} // namespace pembroke