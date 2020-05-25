#include <catch2/catch.hpp>

#include <thread>
#include <type_traits>

#include "pembroke/reactor.hpp"
#include "pembroke/event/timer.hpp"

using namespace std::chrono_literals;
using namespace pembroke::event;

//-----------------------------------------------------------------------------
// Timer Event Tests
//-----------------------------------------------------------------------------

TEST_CASE("Schedule zero-wait timer", "[reacto(r][execution]") {
    auto r = pembroke::reactor().build();
    auto x = 0;

    auto event = TimerEvent(0s, [&]() -> void {
        x += 1;
    });
    CHECK(r->register_event(event));

    r->tick();
    CHECK(x == 1);
}

TEST_CASE("Schedule delayed timer (blocking)", "[reactor][execution]") {
    auto r = pembroke::reactor().build();
    auto x = 0;

    auto event = TimerEvent(100us, [&]() -> void {
        x += 1;
        r->stop();
    });
    CHECK(r->register_event(event));

    r->run_blocking();
    CHECK(x == 1);
}

TEST_CASE("Schedule delayed timer (non-blocking)", "[reactor][execution]") {
    auto r = pembroke::reactor().build();
    auto x = 0;

    auto event = TimerEvent(100us, [&]() -> void {
        x += 1;
    });
    CHECK(r->register_event(event));

    r->tick();
    CHECK(x == 0);

    std::this_thread::sleep_for(10ms);

    r->tick();
    CHECK(x == 1);
}

TEST_CASE("Cancel a scheduled, delayed timer", "[reactor][execution]") {
    auto r = pembroke::reactor().build();
    auto x = 0;

    auto event = TimerEvent(10us, [&]() -> void {
        x += 1;
    });
    CHECK(r->register_event(event));
    CHECK(event.cancel());

    std::this_thread::sleep_for(10ms);
    r->tick();
    CHECK(x == 0);
}

TEST_CASE("Cancel a scheduled, delayed timer before registration", "[reactor][execution]") {
    auto r = pembroke::reactor().build();
    auto x = 0;

    auto event = TimerEvent(10us, [&]() -> void {
        x += 1;
    });
    CHECK(event.cancel());
    CHECK_FALSE(r->register_event(event));

    std::this_thread::sleep_for(10ms);
    r->tick();
    CHECK(x == 0);
}