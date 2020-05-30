#include <catch2/catch.hpp>
#include <chrono>

#include "pembroke/internal/util.hpp"

using namespace pembroke;
using namespace std::chrono_literals;

TEST_CASE("create timeval with sub-second value", "[util][timeval]") {
    auto micros = 50us;
    auto tv = internal::to_timeval(micros);

    CHECK(tv.tv_sec == 0);
    CHECK(tv.tv_usec == 50);
}

TEST_CASE("create timeval with 1 second, no micro-seconds", "[util][timeval]") {
    auto sec = 1s;
    auto tv = internal::to_timeval(sec);

    CHECK(tv.tv_sec == 1);
    CHECK(tv.tv_usec == 0);
}
TEST_CASE("create timeval with exactly 1 second, in millis", "[util][timeval]") {
    auto sec = 1000ms;
    auto tv = internal::to_timeval(sec);

    CHECK(tv.tv_sec == 1);
    CHECK(tv.tv_usec == 0);
}

TEST_CASE("create timeval wit millis", "[util][timeval]") {
    auto sec = 500ms;
    auto tv = internal::to_timeval(sec);

    CHECK(tv.tv_sec == 0);
    CHECK(tv.tv_usec == 500000);
}

TEST_CASE("create timeval with overflow", "[util][timeval]") {
    auto sec = 1500ms;
    auto tv = internal::to_timeval(sec);

    CHECK(tv.tv_sec == 1);
    CHECK(tv.tv_usec == 500000);
}

TEST_CASE("create timeval with minutes", "[util][timeval]") {
    auto mins = 5min;
    auto tv = internal::to_timeval(mins);

    CHECK(tv.tv_sec == 60 * 5);
    CHECK(tv.tv_usec == 0);
}

TEST_CASE("create timeval with min and micros mix", "[util][timeval]") {
    auto time = 5min + 30ms;
    auto tv = internal::to_timeval(time);

    CHECK(tv.tv_sec == 60 * 5);
    CHECK(tv.tv_usec == 30000);
}