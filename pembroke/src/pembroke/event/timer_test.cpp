#include <catch2/catch.hpp>

#include <chrono>
#include <iostream>
#include <thread>

#include "pembroke/logging.hpp"
#include "pembroke/reactor.hpp"
#include "pembroke/event/timer.hpp"

using namespace std::chrono_literals;
using namespace pembroke::event;

TEST_CASE("Scheduled no-delay timer", "[event][timer][execution]") {

    pembroke::logger::register_handler([](pembroke::logger::Level l, std::string_view msg) -> void {
        std::cout << msg << "\n";
    });

    auto r = pembroke::reactor().build();
    auto x = 0;

    auto event = TimerEvent(0us, 10ms, [&]() -> void {
        x += 1;
    });
    CHECK(r->register_event(event));

    r->tick();
    CHECK(x == 1);
}

TEST_CASE("Scheduled timer event (blocking)", "[event][timer][execution]") {
    auto r = pembroke::reactor().build();
    auto x = 0;

    auto event = TimerEvent(100us, [&]() -> void {
        x += 1;
        if (x == 3) {
            r->stop();
        }
    });
    CHECK(r->register_event(event));

    r->run_blocking();
    CHECK(x == 3);
}

TEST_CASE("Schedule timer event (non-blocking)", "[event][delayed][execution]") {
    auto r = pembroke::reactor().build();
    auto x = 0;

    auto event = TimerEvent(100us, 100us, [&]() -> void {
        x += 1;
    });
    CHECK(r->register_event(event));

    r->tick();
    CHECK(x == 0);

    std::this_thread::sleep_for(10ms);

    r->tick();
    CHECK(x == 1);
}

TEST_CASE("Cancel a scheduled, delayed timer", "[event][delayed][execution]") {
    auto r = pembroke::reactor().build();
    auto x = 0;

    auto event = TimerEvent(10us, 10us, [&]() -> void {
        x += 1;
    });
    CHECK(r->register_event(event));
    CHECK(event.cancel());

    std::this_thread::sleep_for(10ms);
    r->tick();
    CHECK(x == 0);
}

TEST_CASE("Cancel a scheduled, delayed timer after first run", "[event][delayed][execution]") {
    auto r = pembroke::reactor().build();
    auto x = 0;

    auto event = TimerEvent(10us, 10us, [&]() -> void {
        x += 1;
    });
    CHECK(r->register_event(event));

    std::this_thread::sleep_for(10ms);
    r->tick();

    CHECK(event.cancel());
    std::this_thread::sleep_for(10ms);
    r->tick();

    // validate event has not re-registered itself after the
    // callback fires (potentially)
    CHECK(event.canceled());

    CHECK(x == 1);
}

TEST_CASE("Cancel a scheduled, delayed timer before registration", "[event][delayed][execution]") {
    auto r = pembroke::reactor().build();
    auto x = 0;

    auto event = TimerEvent(10us, 10us, [&]() -> void {
        x += 1;
    });
    CHECK(event.cancel());
    CHECK_FALSE(r->register_event(event));

    std::this_thread::sleep_for(10ms);
    r->tick();
    CHECK(x == 0);
    CHECK(event.canceled());
}