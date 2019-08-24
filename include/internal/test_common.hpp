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

#include "logging.hpp"

namespace pembroke {

    [[nodiscard]]
    std::tuple<
        std::function<void(logger::Level l, std::string_view msg)>,
        std::shared_ptr<std::unordered_map<logger::Level, std::stringstream>>
    > test_stream_logger() {
        auto logs = std::make_shared<std::unordered_map<logger::Level, std::stringstream>>();
        logs->insert( {logger::Level::Trace, std::stringstream{}});
        logs->insert({logger::Level::Debug, std::stringstream{}});
        logs->insert({logger::Level::Info, std::stringstream{}});
        logs->insert({logger::Level::Warn, std::stringstream{}});
        logs->insert({logger::Level::Error, std::stringstream{}});
        logs->insert({logger::Level::Crit, std::stringstream{}});

        auto f = [logs](logger::Level l, std::string_view msg) -> void {
            (*logs)[l] << msg;
        };

        return std::make_tuple(f, logs);
    }

} // namespace pembroke