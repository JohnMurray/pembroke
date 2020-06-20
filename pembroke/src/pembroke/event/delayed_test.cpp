#include <catch2/catch.hpp>

#include <thread>
#include <type_traits>

#include "pembroke/reactor.hpp"
#include "pembroke/event/delayed.hpp"

using namespace std::chrono_literals;
using namespace pembroke::event;

TEST_CASE("Schedule zero-wait delayed event", "[event][delayed][execution]") {
    auto r = pembroke::reactor().build();
    auto x = 0;

    auto event = DelayedEvent(0s, [&]() -> void {
        x += 1;
    });
    CHECK(r->register_event(event));

    CHECK(r->tick());
    CHECK(x == 1);
}

TEST_CASE("Schedule delayed event (blocking)", "[event][delayed][execution]") {
    auto r = pembroke::reactor().build();
    auto x = 0;

    auto event = DelayedEvent(100us, [&]() -> void {
        x += 1;
        CHECK(r->stop());
    });
    CHECK(r->register_event(event));

    CHECK(r->run_blocking());
    CHECK(x == 1);
}

TEST_CASE("Schedule delayed event (non-blocking)", "[event][delayed][execution]") {
    auto r = pembroke::reactor().build();
    auto x = 0;

    auto event = DelayedEvent(100us, [&]() -> void {
        x += 1;
    });
    CHECK(r->register_event(event));

    CHECK(r->tick());
    CHECK(x == 0);

    std::this_thread::sleep_for(10ms);

    CHECK(r->tick());
    CHECK(x == 1);
}

TEST_CASE("Cancel a scheduled, delayed event", "[event][delayed][execution]") {
    auto r = pembroke::reactor().build();
    auto x = 0;

    auto event = DelayedEvent(10us, [&]() -> void {
        x += 1;
    });
    CHECK(r->register_event(event));
    CHECK(event.cancel());

    std::this_thread::sleep_for(10ms);
    CHECK(r->tick());
    CHECK(x == 0);
}

TEST_CASE("Cancel a scheduled, delayed event before registration", "[event][delayed][execution]") {
    auto r = pembroke::reactor().build();
    auto x = 0;

    auto event = DelayedEvent(10us, [&]() -> void {
        x += 1;
    });
    CHECK(event.cancel());
    CHECK_FALSE(r->register_event(event));

    std::this_thread::sleep_for(10ms);
    CHECK(r->tick());
    CHECK(x == 0);
}