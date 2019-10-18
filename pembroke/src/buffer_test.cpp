#include <catch2/catch.hpp>

#include "pembroke/buffer.hpp"


// ---
// Basic Construction
// ---

TEST_CASE("Buffer is default constructable", "[buffer][construction]") {
    REQUIRE_NOTHROW(pembroke::Buffer());
}

// ---
// Basic Writes
// ---

TEST_CASE("Buffer is writable from std::string", "[buffer][write]") {
    auto b = pembroke::Buffer();

    std::string one{"one"};
    std::string two{"two"};

    REQUIRE_NOTHROW(b.add(one));
    REQUIRE_NOTHROW(b.add(two));

    CHECK(b.length() == 6);
}

TEST_CASE("Buffer is writable from std::string_view", "[buffer][write]") {
    auto b = pembroke::Buffer();

    std::string one{"one"};
    std::string two{"two"};

    REQUIRE_NOTHROW(b.add(std::string_view(one)));
    REQUIRE_NOTHROW(b.add(std::string_view(two)));

    CHECK(b.length() == 6);
}

TEST_CASE("Buffer is writable from char *", "[buffer][write]") {
    auto b = pembroke::Buffer();

    const char *c_str = "Hello, There!";
    REQUIRE_NOTHROW(b.add(c_str));

    CHECK(b.length() == std::string(c_str).length());
}

TEST_CASE("Buffer is writable from char * a sub-str") {
    auto b = pembroke::Buffer();

    char *c_str = (char *)malloc(sizeof(char) * 4);
    c_str[0] = 'w';
    c_str[1] = 'o';
    c_str[2] = 'w';
    c_str[3] = '!';

    REQUIRE_NOTHROW(b.add(c_str, 3));
    CHECK(b.length() == 3);

    REQUIRE_NOTHROW(b.add(c_str, 4));
    CHECK(b.length() == 7);

    REQUIRE_NOTHROW(b.add(&c_str[2], 1));
    CHECK(b.length() == 8);

    free(c_str);
}

// ---
// Basic Reads
// ---