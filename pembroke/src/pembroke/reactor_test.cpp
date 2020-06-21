#include <catch2/catch.hpp>

#include <chrono>
#include <thread>
#include <type_traits>

#include "pembroke/reactor.hpp"
#include "pembroke/event/delayed.hpp"

using namespace std::chrono_literals;
using namespace pembroke::event;


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

static constexpr int ReactorIterations = 10;

TEST_CASE("Reactor runs with no scheduled events", "[reactor][execution]") {
    auto r = pembroke::reactor().build();
    for (int i = 0; i < ReactorIterations; i++) {
        REQUIRE_NOTHROW(r->tick());
        REQUIRE_NOTHROW(r->tick_fast());
    }
}

TEST_CASE("Reactor ticks after post-run-stop", "[reactor][execution]") {
    auto r = pembroke::reactor().build();

    for (int i = 0; i < ReactorIterations; i++) {
        CHECK(r->tick());
        CHECK(r->stop());
    }
}

TEST_CASE("Reactor stops post-run, repeatedly", "[reactor][execution]") {
    auto r = pembroke::reactor().build();
    CHECK(r->tick());

    for (int i = 0; i < ReactorIterations; i++) {
        CHECK(r->stop());
    }
}

// ---
// Stop & Resume (with Events)
// ---

TEST_CASE("Stop. Do not execute pending tasks", "[reactor][execution]") {
    auto r = pembroke::reactor().build();
    auto x = 0;

    auto t1 = DelayedEvent(10us, [&]() -> void { 
        x += 1;
        CHECK(r->stop());
    });
    auto t2 = DelayedEvent(100us, [&]() -> void { x += 1; });
    auto t3 = DelayedEvent(100us, [&]() -> void { x += 1; });
    auto t4 = DelayedEvent(100us, [&]() -> void { x += 1; });

    CHECK(r->register_event(t1));
    CHECK(r->register_event(t2));
    CHECK(r->register_event(t3));
    CHECK(r->register_event(t4));

    std::this_thread::sleep_for(10ms);
    CHECK(r->run_blocking());
    CHECK(x == 1);
}

TEST_CASE("Resume and execute pending tasks", "[reactor][execution]") {
    auto r = pembroke::reactor().build();
    auto x = 0;

    auto t1 = DelayedEvent(0us, [&]() -> void { 
        x += 1;
        CHECK(r->stop());
    });
    auto t2 = DelayedEvent(100us, [&]() -> void { x += 1; });
    auto t3 = DelayedEvent(100us, [&]() -> void { x += 1; });
    auto t4 = DelayedEvent(100us, [&]() -> void { x += 1; });

    CHECK(r->register_event(t1));
    CHECK(r->register_event(t2));
    CHECK(r->register_event(t3));
    CHECK(r->register_event(t4));

    CHECK(r->run_blocking());
    std::this_thread::sleep_for(10ms);
    CHECK(r->tick());
    CHECK(x == 4);
}

TEST_CASE("Schedule events while paused, execute on resumption", "[reactor][execution]") {
    auto r = pembroke::reactor().build();
    auto x = 0;

    std::shared_ptr<DelayedEvent> t2;
    std::shared_ptr<DelayedEvent> t3;
    std::shared_ptr<DelayedEvent> t4;

    auto t1 = DelayedEvent(0us, [&]() -> void { 
        x += 1;
        CHECK(r->stop());
        t2 = std::make_shared<DelayedEvent>(0us, [&]() -> void { x += 1; });
        t3 = std::make_shared<DelayedEvent>(0us, [&]() -> void { x += 1; });
        t4 = std::make_shared<DelayedEvent>(0us, [&]() -> void { x += 1; });

        CHECK(r->register_event(*t2));
        CHECK(r->register_event(*t3));
        CHECK(r->register_event(*t4));
    });
    CHECK(r->register_event(t1));

    CHECK(r->tick()); // stopped on first tick
    CHECK(r->tick()); // execute events schedule post-stop CHECK(x == 4);
}