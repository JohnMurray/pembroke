#pragma once

#include <chrono>

extern "C" {
#include <sys/time.h>
}

/*
 * This file defines some common utilities that are useful to pembroke
 * but aren't genearlly useful outside of this class or at least aren't
 * things that make sense to be part of the public API.
 */

namespace pembroke::internal {

    timeval to_timeval(std::chrono::duration<long, std::micro> &cd);

}  // namespace pembroke::internal