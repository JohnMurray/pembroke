#pragma once

#include <string_view>

namespace pembroke::logger {

    /*
     * Logging definitions that are only to be used internal to Pembroke. These
     * functions are not suitable for application/user-specific logic.
     */

    void trace(std::string_view msg);
    void debug(std::string_view msg);
    void info(std::string_view msg);
    void warn(std::string_view msg);
    void error(std::string_view msg);
    void crit(std::string_view msg);

} // namespace pembroke::logger