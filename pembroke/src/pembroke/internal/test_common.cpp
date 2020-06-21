#include "pembroke/internal/test_common.hpp"

#include <array>
#include <random>
#include <vector>

namespace pembroke {

    auto test_stream_logger() noexcept -> std::tuple<
        std::function<void(logger::Level l, std::string_view msg)>,
        std::shared_ptr<std::unordered_map<logger::Level, std::stringstream>>
    > {
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

    static constexpr uint32_t DistributionUpperLimit = 10000;
    static constexpr uint32_t StackArraySize = 256;
    static constexpr uint32_t StackKbs = 8;

    void trample_stack() noexcept {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist(1,DistributionUpperLimit);

        /* Assume 8kb stack, so produce 16kb of data */
        for (int i = 0; i < StackKbs; i++) {
            std::array<uint32_t, StackArraySize> x{};
            for (int j = 0; j < StackArraySize; j++) {
                x.at(j) = dist(rng);
                if (j > 0) {
                    x.at(j) += x.at(j - 1);
                }
            }
            // do not optimize away
            (void)x;
        }
    }

}  // namespace pembroke