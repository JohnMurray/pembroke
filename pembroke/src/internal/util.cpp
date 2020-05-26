#include "pembroke/internal/util.hpp"

namespace pembroke::internal {

    template<>
    timeval to_timeval(const std::chrono::duration<long, std::micro> &cd) {
        return timeval {
            .tv_sec = static_cast<int>(cd.count() / MICR0S_IN_SECOND),
            .tv_usec = static_cast<int>(cd.count() % MICR0S_IN_SECOND),
        };
    }

} // namespace pembroke::internal