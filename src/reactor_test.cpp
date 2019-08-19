#include <catch2/catch.hpp>

#include "reactor.hpp"

TEST_CASE("Reactor easily constructable", "[reactor][smoke]") {
    REQUIRE_NOTHROW(pembroke::reactor());
    REQUIRE_NOTHROW(pembroke::reactor().build());
}

TEST_CASE("Reactor multiple constructable", "[reactor][smoke]") {
    std::unique_ptr<pembroke::Reactor> r1;
    std::unique_ptr<pembroke::Reactor> r2;

    REQUIRE_NOTHROW(r1 = pembroke::reactor().build());
    REQUIRE_NOTHROW(r2 = pembroke::reactor().build());
}

TEST_CASE("Reactor features-toggles", "[reactor][smoke]") {
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