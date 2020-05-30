#pragma once
/*
 * This file contains some utilities for internal testing code. If you are looking
 * at this file as a user of the library note that while some of what you find may
 * be useful, it is an internal header that is not subject to any sort of semantic
 * versioning or breaking-change expectations you might have. Use at your own risk.
 */
#include <functional>
#include <memory>
#include <sstream>
#include <string_view>
#include <tuple>
#include <unordered_map>

#include <catch2/catch.hpp>

#include "pembroke/logging.hpp"

namespace pembroke {

    [[nodiscard]]
    std::tuple<
        std::function<void(logger::Level l, std::string_view msg)>,
        std::shared_ptr<std::unordered_map<logger::Level, std::stringstream>>
    > test_stream_logger() noexcept;

    void trample_stack() noexcept;

} // namespace pembroke