#include <catch2/catch.hpp>

#include <memory>

#include "pembroke/task.hpp"

static auto nop_ctx = pembroke::TaskContext{};

TEST_CASE("task constructable with lambda", "[task][construction]") {
    int x = 0;
    auto lambda = [&x]() noexcept -> void {
        x += 1;
    };

    CHECK_NOTHROW(pembroke::Task(lambda));
    CHECK_NOTHROW(pembroke::Task([]() {}));
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
    CHECK_NOTHROW(pembroke::Task(&function_pointer));
}

struct CallableObject {
    void operator()() noexcept {}
};
struct CallableObjectConst {
    void operator()() const noexcept {}
};
TEST_CASE("task constructable with callable struct", "[task][construction]") {
    auto co = CallableObject();
    auto coc = CallableObjectConst();

    CHECK_NOTHROW(pembroke::Task(co));
    CHECK_NOTHROW(pembroke::Task(coc));
}

TEST_CASE("run task with no exception", "[task][execution]") {
    auto x = 0;
    auto t = pembroke::Task([&x]() -> void {
        x += 1;
    });
    t.run(nop_ctx);
    CHECK(x == 1);
}

TEST_CASE("run task with exception", "[task][execution]") {
    auto x = 0;
    auto t = pembroke::Task([&x]() -> void {
        x += 1;
        throw std::runtime_error("test_exception");
    });
    CHECK_NOTHROW(t.run(nop_ctx));
    CHECK(x == 1);
}