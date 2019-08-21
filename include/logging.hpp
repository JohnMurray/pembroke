#pragma once

#include <functional>
#include <string_view>

namespace pembroke::logger {

    enum class Level {
        Trace,
        Debug,
        Info,
        Warn,
        Error,
        Crit,
    };

    /**
     * @brief Register a handler that will receive all of the log messages produced by
     *        the pembroke library.
     * @note  This is a utility wrapper for the non-generic `register_handler` function
     *        which is constructed with a std::function.
     */
    template<typename generic_functor>
    void register_handler(generic_functor f) {
        static_assert(
            std::is_constructible_v<
                std::function<void(Level, std::string_view)>,
                generic_functor>,
            "Invalid callable given to 'register_handler'");
        std::function<void(Level, std::string_view)> func {f};
        register_handler(func);
    }

    /**
     * @brief Assign a global log-message handler for all internal pembroke messages
     */
    void register_handler(const std::function<void(Level, std::string_view)> &f);


} // namespace pembroke::logger