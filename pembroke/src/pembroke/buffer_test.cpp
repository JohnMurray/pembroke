#include <catch2/catch.hpp>

#include "pembroke/buffer.hpp"
#include "pembroke/internal/test_common.hpp"


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

TEST_CASE("Buffer is writable from a sub-str of char *", "[buffer][write]") {
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

TEST_CASE("Buffer is writable from a std::byte * bytes array", "[buffer][write]") {
    auto b = pembroke::Buffer();
    std::byte *bytes = (std::byte *)malloc(sizeof(std::byte) * 4);
    REQUIRE_NOTHROW(b.add(bytes, 4));
    CHECK(b.length() == 4);

    REQUIRE_NOTHROW(b.add(bytes, 1));
    CHECK(b.length() == 5);

    REQUIRE_NOTHROW(b.add(&bytes[1], 2));
    CHECK(b.length() == 7);

    free(bytes);
}

TEST_CASE("Buffer is writable from single std::byte", "[buffer][write]") {
    auto b = pembroke::Buffer();
    std::byte byte{75};
    REQUIRE_NOTHROW(b.add(&byte, 1));
    CHECK(b.length() == 1);
}

TEST_CASE("Buffer is writable from ByteSlice", "[buffer][write]") {
    auto b = pembroke::Buffer();
    std::byte *bytes = (std::byte *)malloc(sizeof(std::byte) * 4);

    {
        auto slice = pembroke::ByteSlice{.bytes = bytes, .len = 4};
        REQUIRE_NOTHROW(b.add(slice));
        CHECK(b.length() == 4);
    }

    {
        auto slice = pembroke::ByteSlice{.bytes = bytes, .len = 1};
        REQUIRE_NOTHROW(b.add(slice));
        CHECK(b.length() == 5);
    }

    {
        auto slice = pembroke::ByteSlice{.bytes = bytes + 1, .len = 2};
        REQUIRE_NOTHROW(b.add(slice));
        CHECK(b.length() == 7);
    }

    free(bytes);
}

// ---
// Basic Reads
// ---

TEST_CASE("string written equals string read", "[buffer][read]") {
    auto b = pembroke::Buffer();

    b.add("test");
    CHECK(b.length() == 4);
    CHECK(b.size() == 4);
    CHECK(b.str() == "test");
    CHECK(b.view_str() == "test");
}

TEST_CASE("string written can be returned as a view", "[buffer][read]") {
    auto b = pembroke::Buffer();

    b.add("test");
    std::string_view sv = b.view_str();
    CHECK(sv == "test");
}

TEST_CASE("scoping of written value does not affect underlying buffer", "[buffer][read][write]") {
    auto b = pembroke::Buffer();
    
    {
        std::string str{"Hello, "};
        b.add(str);
    }

    {
        const char *str = "World";
        b.add(str);
    }

    {
        std::byte byte{'!'};
        b.add(&byte, 1);
    }

    /* Even though everything we've written is technically out of scope, write a bunch of
     * junk on the stack just to try and ensure that our buffer is properly copying data
     * and that it can be read back without unexpected surprises */
    pembroke::trample_stack();

    std::string hello{"Hello, World!"};
    CHECK(b.length() == hello.length());
    CHECK(b.str() == hello);
}

TEST_CASE("reading bytes", "[buffer][read]") {
    auto b = pembroke::Buffer();
    std::string s = "Hello, World!";

    b.add(s);
    auto bytes = b.bytes();

    CHECK(bytes.len == s.length());
    for (int i = 0; i < bytes.len; i++) {
        CHECK((char)bytes.bytes[i] == s.data()[i]);
    }
}

// ---
// Move Testing
// ---

TEST_CASE("buffer an be move-constructed",
          "[buffer][move][construction]") {
    pembroke::Buffer b1{};
    b1.add("test string");

    pembroke::Buffer b2{std::move(b1)};

    // Should contain data written to b1
    CHECK(b2.str() == "test string");

    // b1 should be an empty shell
    CHECK(b1.str() == "");
    CHECK(b1.length() == 0);
}

TEST_CASE("buffer retains data after move 1",
          "[buffer][move][construction]") {
    pembroke::Buffer *b;
    {
        auto b_sub = pembroke::Buffer();
        b_sub.add("test string");

        b = new pembroke::Buffer(std::move(b_sub));
    }

    CHECK(b->str() == "test string");
    delete b;
}

TEST_CASE("buffer retains data after move 2",
          "[buffer][move][construction]") {
    pembroke::Buffer b;
    {
        auto b_sub = pembroke::Buffer();
        b_sub.add("test string");

        b = std::move(b_sub);
    }

    CHECK(b.str() == "test string");
}

TEST_CASE("buffer moved _to_ loses data on move-assignment",
          "[buffer][move][construction]") {
    auto b = pembroke::Buffer();
    b.add("short-lived buffer data");

    auto bp = &b;
    {
        auto b_sub = pembroke::Buffer();
        b_sub.add("test string");

        b = std::move(b_sub);
    }

    CHECK(b.str() == "test string");
    CHECK(bp->str() == "test string");
}