#include <catch2/catch.hpp>

#include "reactor.hpp"

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

TEST_CASE("Reactor runs with no scheduled events", "[reactor][execution]") {
    auto r = pembroke::reactor().build();
    REQUIRE_NOTHROW(r->run_non_blocking());
}

TEST_CASE("Reactor resumes post-run, repeatedly", "[reactor][execution]") {
    auto r = pembroke::reactor().build();
    r->run_non_blocking();

    CHECK(r->resume());
    CHECK(r->resume());
    CHECK(r->resume());
}

TEST_CASE("Reactor pauses post-run, repeatedly", "[reactor][execution]") {
    auto r = pembroke::reactor().build();
    r->run_non_blocking();

    CHECK(r->pause());
    CHECK(r->pause());
    CHECK(r->pause());
}

// TODO: Using tasks, validate the behavior:
//       - pause()             - Ensure pause actually pauses a reactor from within a task
//       - resume()            - Ensure resume actually resumes from a paused task (paused from a task)
//                             - Ensure pending tasks are run on resume (that were scheduled during a pause)
//       - run_non_blocking()  - Ensure pending tasks are executed and while-running tasks are not until the next method invocation
//       - run_blocking()      - Ensure pending tasks and while-running tasks are executed
//       - tick()              - Ensure same behavior as run_non_blocking()