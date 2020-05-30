#pragma once

#include <chrono>

namespace pembroke {
    using duration = std::chrono::duration<int64_t, std::micro>;

    constexpr auto no_delay = duration(0);

}