#include <catch2/catch.hpp>

#include <chrono>
#include <thread>
#include <type_traits>

#include "pembroke/reactor.hpp"

using namespace std::chrono_literals;


// ---
// Basic Construction and Builder Configurations
// ---

TEST_CASE("Reactor easily constructable", "[reactor][construction]") {
    REQUIRE_NOTHROW(pembroke::reactor());
    REQUIRE_NOTHROW(pembroke::reactor().build());
}

TEST_CASE("Reactor multiple constructable", "[reactor][construction]") {
    std::unique_ptr<pembroke::Reactor> r1;
    std::unique_ptr<pembroke::Reactor> r2;

    REQUIRE_NOTHROW(r1 = pembroke::reactor().build());
    REQUIRE_NOTHROW(r2 = pembroke::reactor().build());
}

TEST_CASE("Reactor features-toggles", "[reactor][construction]") {
    auto builder = pembroke::reactor();

    SECTION("toggle all (on by default)") {
        REQUIRE_NOTHROW(builder.require_edge_trigger_support());
        REQUIRE_NOTHROW(builder.require_file_descriptor_support());
        REQUIRE_NOTHROW(builder.require_early_close_support());
        REQUIRE_NOTHROW(builder.require_order_one_trigger_support());

        CHECK(builder.m_require_edge_triggering);
        CHECK(builder.m_require_file_descriptor);
        CHECK(builder.m_require_early_close);
        CHECK(builder.m_require_order_one_trigger);
    }

    SECTION("Toggle Selectively Off") {
        REQUIRE_NOTHROW(builder.require_edge_trigger_support(false));
        CHECK_FALSE(builder.m_require_edge_triggering);

        REQUIRE_NOTHROW(builder.require_file_descriptor_support(false));
        CHECK_FALSE(builder.m_require_file_descriptor);

        REQUIRE_NOTHROW(builder.require_early_close_support(false));
        CHECK_FALSE(builder.m_require_early_close);

        REQUIRE_NOTHROW(builder.require_order_one_trigger_support(false));
        CHECK_FALSE(builder.m_require_order_one_trigger);
    }
}

TEST_CASE("Reactor cannot be copied", "[reactor][construction]") {
    CHECK_FALSE(std::is_copy_assignable<pembroke::Reactor>::value);
    CHECK_FALSE(std::is_copy_constructible<pembroke::Reactor>::value);
}

TEST_CASE("Reactor cannot be moved", "[reactor][construction]") {
    CHECK_FALSE(std::is_move_assignable<pembroke::Reactor>::value);
    CHECK_FALSE(std::is_move_constructible<pembroke::Reactor>::value);
}

// ---
// No-Op Execution
// ---

TEST_CASE("Reactor runs with no scheduled events", "[reactor][execution]") {
    auto r = pembroke::reactor().build();
    for (int i = 0; i < 10; i++) {
        REQUIRE_NOTHROW(r->tick());
        REQUIRE_NOTHROW(r->tick_fast());
    }
}

TEST_CASE("Reactor ticks after post-run-stop", "[reactor][execution]") {
    auto r = pembroke::reactor().build();

    for (int i = 0; i < 10; i++) {
        CHECK(r->tick());
        CHECK(r->stop());
    }
}

TEST_CASE("Reactor stops post-run, repeatedly", "[reactor][execution]") {
    auto r = pembroke::reactor().build();
    r->tick();

    for (int i = 0; i < 10; i++) {
        CHECK(r->stop());
    }
}

// ---
// Scheduled Events
// ---

TEST_CASE("Schedule zero-wait timer", "[reactor][execution]") {
    auto r = pembroke::reactor().build();
    auto x = 0;

    r->new_timer([&]() -> void {
        x += 1;
    }, 0s);

    r->tick();
    CHECK(x == 1);
}

TEST_CASE("Schedule delayed timer (blocking)", "[reactor][execution]") {
    auto r = pembroke::reactor().build();
    auto x = 0;

    r->new_timer([&]() -> void {
        x += 1;
        r->stop();
    }, 100us);

    r->run_blocking();
    CHECK(x == 1);
}

TEST_CASE("Schedule delayed timer (non-blocking)", "[reactor][execution]") {
    auto r = pembroke::reactor().build();
    auto x = 0;

    r->new_timer([&]() -> void {
        x += 1;
    }, 100us);

    r->tick();
    CHECK(x == 0);

    std::this_thread::sleep_for(10ms);

    r->tick();
    CHECK(x == 1);
}

TEST_CASE("Cancel a scheduled, delayed timer", "[reactor][execution]") {
    auto r = pembroke::reactor().build();
    auto x = 0;

    auto e = r->new_timer([&]() -> void {
        x += 1;
    }, 10us);
    e.cancel();

    std::this_thread::sleep_for(10ms);
    r->tick();
    CHECK(x == 0);
}

// ---
// Stop & Resume (with Events)
// ---

TEST_CASE("Stop. Do not execute pending tasks", "[reactor][execution]") {
    auto r = pembroke::reactor().build();
    auto x = 0;

    r->new_timer([&]() -> void { 
        x += 1;
        r->stop();
    }, 10us);
    r->new_timer([&]() -> void { x += 1; }, 100us);
    r->new_timer([&]() -> void { x += 1; }, 100us);
    r->new_timer([&]() -> void { x += 1; }, 100us);

    std::this_thread::sleep_for(10ms);
    r->run_blocking();
    CHECK(x == 1);
}

TEST_CASE("Resume and execute pending tasks", "[reactor][execution]") {
    auto r = pembroke::reactor().build();
    auto x = 0;

    r->new_timer([&]() -> void { 
        x += 1;
        r->stop();
    }, 0us);
    r->new_timer([&]() -> void { x += 1; }, 100us);
    r->new_timer([&]() -> void { x += 1; }, 100us);
    r->new_timer([&]() -> void { x += 1; }, 100us);

    r->run_blocking();
    std::this_thread::sleep_for(10ms);
    r->tick();
    CHECK(x == 4);
}

TEST_CASE("Schedule events while paused, execute on resumption", "[reactor][execution]") {
    auto r = pembroke::reactor().build();
    auto x = 0;

    r->new_timer([&]() -> void { 
        x += 1;
        r->stop();
        r->new_timer([&]() -> void { x += 1; }, 0us);
        r->new_timer([&]() -> void { x += 1; }, 0us);
        r->new_timer([&]() -> void { x += 1; }, 0us);
    }, 0us);

    r->tick(); // stopped on first tick
    r->tick(); // execute events schedule post-stop
    CHECK(x == 4);
}