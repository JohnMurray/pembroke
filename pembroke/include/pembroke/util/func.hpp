#pragma once

namespace pembroke::util {
    constexpr auto nop_f = []() -> void {};

    /**
     * @brief Generate a NOP function with a static return value
     * 
     * @b Example
     * @code{.cpp}
     *   auto true_func  = pembroke::util::nop_f_r<bool, true>();
     *   auto false_func = pembroke::util::nop_f_r<bool, false();
     * 
     *   true_func();  // => true
     *   false_func(); // => false
     * @endcode
     */
    template<typename T, T returnVal>
    constexpr auto nop_f_r() {
        return []() -> T { return returnVal; };
    }
}