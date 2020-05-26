#pragma once

#include <chrono>

namespace pembroke {

    constexpr auto no_delay = std::chrono::duration<int64_t, std::micro>(0);

}