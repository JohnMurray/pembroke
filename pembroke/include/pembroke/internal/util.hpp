#pragma once

#include <cstdlib>
#include <chrono>
#include <fmt/format.h>

extern "C" {
#include <sys/time.h>
}

/*
 * This file defines some common utilities that are useful to pembroke
 * but aren't genearlly useful outside of this library or at least aren't
 * things that make sense to be part of the public API.
 */

namespace pembroke::internal {

    constexpr static int MILLIS_IN_SECOND = 1000000;

    /**
     * @brief Convert a chrono duration into a C-style timeval struct
     * @returns A timeval struct by value
     */
    template<typename T>
    timeval to_timeval(const std::chrono::duration<long, T> &cd) {
        std::chrono::duration<long, std::micro> micros =
            std::chrono::duration_cast<std::chrono::microseconds>(cd);
        return to_timeval(micros);
    }

    /**
     * @brief Specialization to work specifically with micros. All other
     *        templates will call down to this one.
     * @returns A timeval struct by value
     */
    template<>
    timeval to_timeval(const std::chrono::duration<long, std::micro> &cd);

}  // namespace pembroke::internal



//-----------------------------------------------------------------------------
// Assert macros

/* Assertion macro that boils down to a static assert and does not compile away
 * for production builds. Represents invariants. */
#define ASSERT_RELEASE(cond, msg)                 \
    do {                                          \
        if (!(cond)) {                              \
            auto error = fmt::format(             \
                "assert failure: ({}) {}",        \
                #cond, msg);                      \
            pembroke::logger::crit(error);        \
            std::abort();                         \
        }                                         \
    } while(false);


/* Debug assertions represent things that SHOULD NOT happen under normal circumstances
 * but may happen in real-world deployments. These assertions are compiled out of
 * optimzied builds via NDEBUG */
#ifdef NDEBUG
#define ASSERT_DEBUG(cond, msg)
#else
#define ASSERT_DEBUG(cond, msg) ASSERT_RELEASE((cond), (msg))
#endif