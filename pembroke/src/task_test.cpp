#include <catch2/catch.hpp>

#include "pembroke/task.hpp"

TEST_CASE("task constructable with lambda", "[task][construction]") {
    int x = 0;
    CHECK_NOTHROW(pembroke::Task([&x]() -> void {
        x += 1;
    }));

    CHECK_NOTHROW(pembroke::Task([](){}));
}

TEST_CASE("task constructable with std::function", "[task][construction]") {
    int x = 0;
    std::function<void()> f = [&x]() -> void {
        x += 1;
    };
    auto &rf = f;
    const auto &crf = f;
    CHECK_NOTHROW(pembroke::Task(f));
    CHECK_NOTHROW(pembroke::Task(rf));
    CHECK_NOTHROW(pembroke::Task(crf));
}

void function_pointer() {}
TEST_CASE("task constructable with function pointer", "[task][construction]") {
    CHECK_NOTHROW(pembroke::Task(function_pointer));
}

TEST_CASE("run task with no exception", "[task][execution]") {
    auto x = 0;
    auto t = pembroke::Task([&x]() -> void {
        x += 1;
    });
    t.run();
    CHECK(x == 1);
}

TEST_CASE("run task with exception", "[task][execution]") {
    auto x = 0;
    auto t = pembroke::Task([&x]() -> void {
        x += 1;
        throw std::runtime_error("test_exception");
    });
    CHECK_NOTHROW(t.run());
    CHECK(x == 1);
}

TEST_CASE("run task with non-standard exception", "[task][execution]") {
    auto x = 0;
    auto t = pembroke::Task([&x]() -> void {
        x += 1;
        throw "test_exception";
    });
    CHECK_NOTHROW(t.run());
    CHECK(x == 1);
}