#pragma once

#include <string_view>

namespace pembroke::logger {

    void trace(const std::string_view msg);
    void debug(const std::string_view msg);
    void info(const std::string_view msg);
    void warn(const std::string_view msg);
    void error(const std::string_view msg);
    void crit(const std::string_view msg);

} // namespace pembroke::logger