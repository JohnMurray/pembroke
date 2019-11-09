#include "pembroke/internal/test_common.hpp"

#include <random>
#include <vector>

namespace pembroke {

    std::tuple<
        std::function<void(logger::Level l, std::string_view msg)>,
        std::shared_ptr<std::unordered_map<logger::Level, std::stringstream>>
    > test_stream_logger() noexcept {
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

    void trample_stack() noexcept {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist(1,10000);

        /* Assume 8kb stack, so produce 16kb of data */
        for (int i = 0; i < 8; i++) {
            uint32_t x[256];
            for (int j = 0; j < 256; j++) {
                x[j] = dist(rng);
                if (j > 0) x[j] += x[j - 1];
            }
            // do not optimize away
            (void)x;
        }
    }

}  // namespace pembroke