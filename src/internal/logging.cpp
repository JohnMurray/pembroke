#include "logging.hpp"
#include "internal/logging.hpp"

namespace pembroke::logger {

    /**
     * @brief Global log handler (default initialized to a no_op handler)
     */
    static std::function<void(Level, std::string_view)> _log_handler = 
        [](Level, std::string_view) -> void {};

    void register_handler(const std::function<void(Level, std::string_view)> &f) {
        _log_handler = f;
    }

    void trace(const std::string_view msg) {
        _log_handler(Level::Trace, msg);
    }
    void debug(const std::string_view msg) {
        _log_handler(Level::Debug, msg);
    }
    void info(const std::string_view msg) {
        _log_handler(Level::Info, msg);
    }
    void warn(const std::string_view msg) {
        _log_handler(Level::Warn, msg);
    }
    void error(const std::string_view msg) {
        _log_handler(Level::Error, msg);
    }
    void crit(const std::string_view msg) {
        _log_handler(Level::Crit, msg);
    }

} // namespace pembroke::logger