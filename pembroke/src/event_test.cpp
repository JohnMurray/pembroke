#include <catch2/catch.hpp>

#include <type_traits>

#include "event.hpp"

/*
 * The event is a very simple class that is closely related to the reactor. As such,
 * most event testing is done through testing of the reactor. See reactor tests for
 * more details.
 */

TEST_CASE("Event cannot be copied", "[event][construction]") {
        CHECK_FALSE(std::is_copy_constructible<pembroke::EventContext>::value);
        CHECK_FALSE(std::is_copy_assignable<pembroke::EventContext>::value);
}

TEST_CASE("Event cannot be moved", "[event][construction]") {
        CHECK_FALSE(std::is_move_constructible<pembroke::EventContext>::value);
        CHECK_FALSE(std::is_move_assignable<pembroke::EventContext>::value);
}