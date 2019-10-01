#include <catch2/catch.hpp>
#include <string>
#include <string_view>

#include "logging.hpp"
#include "internal/logging.hpp"
#include "internal/test_common.hpp"

using namespace pembroke;

TEST_CASE("init logging with lambda", "[logging][construction]") {
    logger::register_handler([](logger::Level l, std::string_view msg) -> void {
        CHECK("test-message" == msg);
        CHECK(logger::Level::Info == l);
    });

    logger::info("test-message");
}

void test_log_handler(logger::Level l, std::string_view msg) {
    CHECK("test-message" == msg);
    CHECK(logger::Level::Warn == l);
}
TEST_CASE("init logging with function pointer", "[logging][construction]") {
    logger::register_handler(test_log_handler);
    logger::warn("test-message");
}

TEST_CASE("multiple log messages", "[logging]") {
    auto [log_f, loggers] = test_stream_logger();
    logger::register_handler(log_f);

    logger::info("this ");
    logger::info("is ");
    logger::info("a ");
    logger::info("test");

    logger::trace("also ");
    logger::trace("a ");
    logger::trace("test");

    CHECK((*loggers)[logger::Level::Info].str() == "this is a test");
    CHECK((*loggers)[logger::Level::Trace].str() == "also a test");
}